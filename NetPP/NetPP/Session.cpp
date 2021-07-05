#include "base.h"

std::unique_ptr<IOCPSession> Singleton<IOCPSession>::sInstance;

IOCPSession::IOCPSession()
{
}

bool IOCPSession::StaticInitialize()
{
	sInstance.reset(new IOCPSession());
	return true;
}

bool IOCPSession::Initialize()
{

	if (IOCPNetworkManager::StaticInit(SERVERPORT))
	{
		{// ���⼭ ���� �Ŵ����� �ʱ�ȭ
			if (false == ClientManager::StaticInit())
				return false;
			if (false == PacketManager::StaticInit())
				return false;
		}// �Ŵ��� �ʱ�ȭ end

		{// callback ����
			IOCPNetworkManager::sInstance->OnCompleteAccept = &OnAccept;
			IOCPNetworkManager::sInstance->OnCompleteRecv = &OnCompleteRecv;
			IOCPNetworkManager::sInstance->OnCompleteSend = &OnCompleteSend;
			IOCPNetworkManager::sInstance->OnDisconnected = &OnDisconnected;
		}// end callback link

		// base
		Session::Initialize();

		return true;
	}
	return false;
}

void IOCPSession::Finalize()
{
	// ...

	// base
	Session::Finalize();
}

IOCPSession::~IOCPSession()
{
}

bool IOCPSession::MainLoop()
{

	while (true)
	{

	}

	return true;
}

bool IOCPSession::OnAccept(TCPSocketPtr inpClientSock, SocketAddress inAddr)
{
	printf("...OnAccept()");

	AutoLocker locker(&IOCPSession::sInstance->m_cs);

	ClientInfoPtr newClient = std::make_shared<ClientInfo>(inpClientSock, inAddr);
	ClientManager::sInstance->RegistNewClient(newClient);
	SocketUtil::LinkIOCPThread(newClient);

	RecvPacketPtr pOutPacket;
	if (false == IOCPNetworkManager::RecvAsync(inpClientSock, pOutPacket))
	{
		// ...
	}

	return true;
}

bool IOCPSession::OnCompleteRecv(InputMemoryStreamPtr)
{
	printf("...OnCompleteRecv()");
	return true;
}

bool IOCPSession::OnCompleteSend(OutputMemoryStreamPtr)
{
	printf("...OnCompleteSend()");
	return true;
}

void IOCPSession::OnDisconnected(ClientInfoPtr)
{
	printf("...OnDisconnected");
}
