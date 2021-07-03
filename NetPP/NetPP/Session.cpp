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
	AutoLocker locker(&IOCPSession::sInstance->m_cs);
	
	ClientInfoPtr newClient = std::make_shared<ClientInfo>(inpClientSock, inAddr);
	ClientManager::sInstance->RegistNewClient(newClient);
	SocketUtil::LinkIOCPThread(newClient);

	return true;
}

bool IOCPSession::OnCompleteRecv(InputMemoryStreamPtr)
{
	return true;
}

bool IOCPSession::OnCompleteSend(OutputMemoryStreamPtr)
{
	return true;
}

void IOCPSession::OnDisconnected(ClientInfoPtr)
{
}
