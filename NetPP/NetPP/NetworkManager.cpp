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
	SocketAddress addr;
	// block accept
	TCPSocketPtr pClientSock = m_pListenSock->Accept(addr);

	// set new client
	ClientInfoPtr newClient = std::make_shared<ClientInfo>(pClientSock, addr);
	ClientManager::sInstance->RegistNewClient(newClient);

	SocketUtil::LinkIOCPThread(newClient);

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

bool NetworkManager::Init(u_short inPort)
{
	// iocp 涝免仿 器飘 积己
	m_pHcp = SocketUtil::CreateIOCP(WorkerThread);
	if (m_pHcp.get() == nullptr)
		return false;

	// listen 家南 积己
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
	
	return true;
}