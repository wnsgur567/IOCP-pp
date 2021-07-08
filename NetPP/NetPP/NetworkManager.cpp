#include "base.h"

std::unique_ptr<IOCPNetworkManager> IOCPNetworkManager::sInstance;

bool NetworkManagerServer::Init(u_short inPort, bool isInNonBlock)
{
	// listen 소켓 생성
	m_pListenSock = SocketUtil::CreateTCPSocket();
	if (m_pListenSock == nullptr)
	{
		return false;
	}
		
	/*if (false == m_pListenSock->SetNonBlockingMode(isInNonBlock))
		return false;*/

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

NetworkManagerServer::~NetworkManagerServer()
{
	SocketUtil::CleanUp();
}


TCPSocketPtr NetworkManagerServer::GetListenSockPtr() const
{
	return m_pListenSock;
}




bool IOCPNetworkManager::Init(u_short inPort, bool isInNonBlock = false)
{
	// iocp 입출력 포트 생성
	m_pHcp = SocketUtil::CreateIOCP(IOCPNetworkManager::WorkerThread, m_hWorkerThreads);
	if (m_pHcp.get() == nullptr)
		return false;

	// parent init
	if (false == NetworkManagerServer::Init(inPort, isInNonBlock))
		return false;

	// create accept thread
	HANDLE hThread = CreateThread(NULL, 0,
		AcceptThread,
		&IOCPNetworkManager::sInstance->m_pListenSock,
		0, NULL);

	m_hAcceptThreads.push_back(std::make_shared<HANDLE>(hThread));

	return true;
}

bool IOCPNetworkManager::StaticInit(u_short inPort)
{
	// singleton init
	sInstance.reset(new IOCPNetworkManager());

	// wsa init
	if (false == SocketUtil::Init())
		return false;

	// init
	return sInstance->Init(inPort);
}

IOCPNetworkManager::~IOCPNetworkManager()
{
}

bool IOCPNetworkManager::DoFrame()
{
	// send process
	if (!SendQueueProcess())
		return false;

	return true;
}

HandlePtr IOCPNetworkManager::GetHCPPtr() const
{
	return m_pHcp;
}

bool IOCPNetworkManager::PushSendQueue(TCPSocketPtr inpSock, SendPacketPtr inpSendPacket)
{
	m_sendQueue.push({ inpSock,inpSendPacket });
	return true;
}

bool IOCPNetworkManager::SendQueueProcess()
{
	while (false == m_sendQueue.empty())
	{
		std::pair<TCPSocketPtr, SendPacketPtr> item = m_sendQueue.front();
		m_sendQueue.pop();

		if (false == IOCPNetworkManager::SendAsync(item.first, item.second))
			return false;
	}

	return true;
}


DWORD __stdcall IOCPNetworkManager::AcceptThread(LPVOID arg)
{
	SocketAddress addr;
	TCPSocket* listen_sock = (TCPSocket*)arg;

	while (true)
	{
		// accept
 		TCPSocketPtr pClientSock = listen_sock->Accept(addr);

		if(pClientSock != nullptr)
		{// lock
			AutoLocker locker(IOCPSession::sInstance->GetCSPtr());

			// post to iocp queue
			AcceptPacketPtr pAcceptPacket = PacketManager::sInstance->GetAcceptPacketFromPool();
			PostQueuedCompletionStatus(
				*IOCPNetworkManager::sInstance->m_pHcp,
				1,
				(ULONG_PTR)&pAcceptPacket,
				&pAcceptPacket->m_overlappedEx.overlapped);
		}// lock end
	}

	return 0;
}

bool IOCPNetworkManager::RecvAsync(const TCPSocketPtr inpSock, RecvPacketPtr& outRecvPacket)
{
	int retval;
	DWORD recvbytes;
	DWORD flags = 0;

	// 풀링된 패킷을 꺼내오기
	RecvPacketPtr pPacket = PacketManager::sInstance->GetRecvPacketFromPool();

	// overlap wsa 준비셋팅
	pPacket->GetReady();

	// 비동기 async 수행
	retval = WSARecv(inpSock->GetSock(),
		&pPacket->m_wsabuf, 1, &recvbytes, &flags,
		&pPacket->m_overlappedEx.overlapped, nullptr);

	if (retval == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			SocketUtil::ReportError("PacketManager::RecvAsync()");
			return false;
		}
	}

	/*if (outRecvPacket != nullptr)
	}
	{*/
	outRecvPacket = pPacket;

	return true;
}

bool IOCPNetworkManager::SendAsync(const TCPSocketPtr inpSock, SendPacketPtr inSendPacket)
{
	int retval;
	DWORD sendbytes;
	DWORD flags = 0;

	inSendPacket->GetReady(PacketManager::sInstance->m_newSendPacketID++);

	retval = WSASend(inpSock->GetSock(),
		&inSendPacket->m_wsabuf, 1, &sendbytes,
		0, &inSendPacket->m_overlappedEx.overlapped, nullptr);

	if (retval == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			SocketUtil::ReportError("PacketManager::SendAsync()");
			return false;
		}
	}

	return true;
}

E_PacketState IOCPNetworkManager::CompleteRecv(TCPSocketPtr inpSock, RecvPacketPtr& inoutpPacket, const PacketBase::psize_t inCompletebyte)
{
	if (inoutpPacket->m_sizeflag)
	{
		inoutpPacket->m_recvbytes += inCompletebyte;

		if (inoutpPacket->m_recvbytes == sizeof(PacketBase::psize_t))
		{
			memcpy(&inoutpPacket->m_target_recvbytes, inoutpPacket->m_buf, sizeof(PacketBase::psize_t));
			inoutpPacket->m_recvbytes = 0;
			inoutpPacket->m_sizeflag = false;
		}

		if (!IOCPNetworkManager::RecvAsync(inpSock, inoutpPacket))
		{
			return E_PacketState::Error;
		}

		return E_PacketState::InComplete;
	}

	inoutpPacket->m_recvbytes += inCompletebyte;

	if (inoutpPacket->m_recvbytes != inoutpPacket->m_target_recvbytes)
	{
		if (!IOCPNetworkManager::RecvAsync(inpSock, inoutpPacket))
		{
			return E_PacketState::Error;
		}
		return E_PacketState::InComplete;
	}

	return E_PacketState::Completed;
}

E_PacketState IOCPNetworkManager::CompleteSend(TCPSocketPtr inpSock, SendPacketPtr inpPacket, const PacketBase::psize_t inCompletebyte)
{
	inpPacket->m_sendbytes += inCompletebyte;
	if (inpPacket->m_sendbytes != inpPacket->m_target_sendbytes)
	{
		if (!IOCPNetworkManager::SendAsync(inpSock, inpPacket))
		{
			return E_PacketState::Error;
		}
		return E_PacketState::InComplete;
	}
	return E_PacketState::Completed;
}

DWORD __stdcall IOCPNetworkManager::WorkerThread(LPVOID arg)
{
	int retval;
	HANDLE hcp = (HANDLE)arg;

	while (true)
	{
		DWORD cbTransferred;
		ClientInfo::id_t client_id;
		OverlappedEx* overlapped;

		retval = GetQueuedCompletionStatus(hcp, &cbTransferred,
			(LPDWORD)&client_id, (LPOVERLAPPED*)&overlapped, INFINITE);

		ClientInfoPtr client_info = ClientManager::sInstance->GetClientInfo(client_id);
		PacketBaseWeakPtr pPacket = overlapped->pPacket;

		// 비동기 입출력 결과 확인
		if (retval == 0 || cbTransferred == 0)
		{
			if (retval == 0)
			{
				DWORD temp1, temp2;
				WSAGetOverlappedResult(client_info->GetSockPtr()->GetSock(),
					&overlapped->overlapped,
					&temp1, FALSE, &temp2);
				SocketUtil::ReportError("WSAGetOverlappedResult()");
			}

			client_info->SetState(E_ClientState::Disconnected);
		}

		if (overlapped->type != E_OverlappedType::Accept &&
			client_info->GetState() == E_ClientState::Disconnected)
		{
			//RemoveClientInfo(ptr);
			// callback
			IOCPNetworkManager::sInstance->OnDisconnected(client_info);
			continue;
		}

		E_PacketState result;

		switch (overlapped->type)
		{
		case E_OverlappedType::Accept:
		{
			// down casting
			AcceptPacketPtr pAcceptPacket = std::static_pointer_cast<AcceptPacket>(pPacket.lock());
			// callback
			IOCPNetworkManager::sInstance->OnCompleteAccept(pAcceptPacket->m_pClientSock, pAcceptPacket->m_sockAddr);
		}
		break;

		case  E_OverlappedType::Recv:
		{
			// down casting
			RecvPacketPtr pRecvPacket = std::static_pointer_cast<RecvPacket>(pPacket.lock());

			// recv 완료 확인
			result = IOCPNetworkManager::CompleteRecv(
				client_info->GetSockPtr(),
				pRecvPacket,
				cbTransferred);
			switch (result)
			{
			case E_PacketState::Error:
				continue;
			case E_PacketState::End:
				continue;
			case E_PacketState::InComplete:
				continue;
			case E_PacketState::Completed:
				// 완료된 경우 시간을 기록
				pRecvPacket->RecordRecvTime();
				break;
			}

			// complete recv process
			IOCPNetworkManager::sInstance->OnCompleteRecv(pRecvPacket->GetStream());

			PacketManager::sInstance->RetrieveRecvPacket(pRecvPacket);

			// recv 날려놓기
			RecvPacketPtr pOutRecvPacket;
			if (!RecvAsync(client_info->GetSockPtr(),
				pOutRecvPacket))
			{
				continue;
			}
		}
		break;

		case E_OverlappedType::Send:
		{
			// down casting
			SendPacketPtr pSendPacket = std::static_pointer_cast<SendPacket>(pPacket.lock());

			result = IOCPNetworkManager::CompleteSend(
				client_info->GetSockPtr(),
				std::static_pointer_cast<SendPacket>(pPacket.lock()),
				cbTransferred);

			switch (result)
			{
			case E_PacketState::Error:
				continue;
			case E_PacketState::End:
				continue;
			case E_PacketState::InComplete:
				continue;
			case E_PacketState::Completed:
				break;
			}

			// complete send process;
			IOCPNetworkManager::sInstance->OnCompleteSend(nullptr);

			// TODO : Retrieve packet
			PacketManager::sInstance->RetrieveSendPacket(pSendPacket);
		}
		break;
		}
	}

	return 0;
}
