#include "base.h"

std::unique_ptr<NetworkManager> NetworkManager::sInstance;

bool NetworkManager::StaticInit(u_short inPort)
{
	sInstance.reset(new NetworkManager());
	if (false == SocketUtil::Init())
		return false;
	return sInstance->Init(inPort);
}

NetworkManager::~NetworkManager()
{
	SocketUtil::CleanUp();
}

bool NetworkManager::DoFrame()
{
	if (!SendQueueProcess())
		return false;

	return true;
}

HandlePtr NetworkManager::GetHCPPtr() const
{
	return m_pHcp;
}

TCPSocketPtr NetworkManager::GetListenSockPtr() const
{
	return m_pListenSock;
}

bool NetworkManager::PushSendQueue(TCPSocketPtr inpSock, SendPacketPtr inpSendPacket)
{
	m_sendQueue.push({ inpSock,inpSendPacket });
	return true;
}

bool NetworkManager::SendQueueProcess()
{
	while (false == m_sendQueue.empty())
	{
		std::pair<TCPSocketPtr, SendPacketPtr> item = m_sendQueue.front();
		m_sendQueue.pop();

		if (false == PacketManager::SendAsync(item.first, item.second))
			return false;
	}

	return true;
}

DWORD __stdcall NetworkManager::AcceptThread(LPVOID arg)
{
	SocketAddress addr;
	TCPSocket* listen_sock = (TCPSocket*)arg;

	while (true)
	{
		// block accept
		TCPSocketPtr pClientSock = listen_sock->Accept(addr);

		// set new client
		ClientInfoPtr newClient = std::make_shared<ClientInfo>(pClientSock, addr);
		ClientManager::sInstance->RegistNewClient(newClient);
		SocketUtil::LinkIOCPThread(newClient);

		// 최초 recv 날리기
		RecvPacketPtr pTmpPacket;
		PacketManager::RecvAsync(pClientSock, pTmpPacket);
	}

	return 0;
}

bool NetworkManager::Init(u_short inPort)
{
	// iocp 입출력 포트 생성
	m_pHcp = SocketUtil::CreateIOCP(PacketManager::WorkerThread);
	if (m_pHcp.get() == nullptr)
		return false;

	// listen 소켓 생성
	m_pListenSock = SocketUtil::CreateTCPSocket();
	if (m_pListenSock == nullptr)
	{
		return false;
	}

	// binding
	SocketAddress myAddress(htonl(INADDR_ANY), htons(inPort));
	if (false == m_pListenSock->Bind(myAddress))
		return false;

	// debug
	printf("Initializing NetworkManager at port %d", inPort);

	// listening
	if (false == m_pListenSock->Listen(SOMAXCONN))
		return false;

	// create accept thread
	HANDLE hThread = CreateThread(NULL, 0, NetworkManager::AcceptThread, m_pListenSock.get(), 0, NULL);


	return true;
}
