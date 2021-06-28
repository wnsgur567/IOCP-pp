#include "base.h"

std::unique_ptr<PacketManager> PacketManager::sInstance;

bool PacketManager::Init(psize_t inStreamPoolingCapacity)
{
	for (psize_t i = 0; i < inStreamPoolingCapacity; i++)
	{
		RecvPacketPtr r_ptr = std::make_shared<RecvPacket>(BUFSIZE);
		r_ptr->Init(r_ptr);
		m_recvpacket_pool.push(r_ptr);
		SendPacketPtr s_ptr = std::make_shared<SendPacket>(BUFSIZE);
		s_ptr->Init(s_ptr);
		m_sendpacket_pool.push(s_ptr);
	}
	return true;
}

bool PacketManager::StaticInit()
{
	// stream 을 capacity 만큼 생성 및 풀링
	return PacketManager::sInstance->Init(STREAMPOOLCAPACITY);
}

PacketManager::~PacketManager()
{
}



void PacketManager::RequestSend(const TCPSocketPtr inpSock, SendPacketPtr inpSendPacket)
{
	NetworkManager::sInstance->PushSendQueue(inpSock, inpSendPacket);
}

bool PacketManager::RecvAsync(const TCPSocketPtr inpSock, RecvPacketPtr outRecvPacket)
{
	int retval;
	DWORD recvbytes;
	DWORD flags = 0;

	outRecvPacket->GetReady();

	retval = WSARecv(inpSock->GetSock(),
		&outRecvPacket->m_wsabuf, 1, &recvbytes, &flags,
		&outRecvPacket->m_overlappedEx.overlapped, nullptr);

	if (retval == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			SocketUtil::ReportError("PacketManager::RecvAsync()");
			return false;
		}
	}
	return true;
}

bool PacketManager::SendAsync(const TCPSocketPtr inpSock, SendPacketPtr inSendPacket)
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

E_PacketState PacketManager::CompleteRecv(TCPSocketPtr inpSock, RecvPacketPtr outpPacket, const PacketBase::psize_t inCompletebyte)
{
	if (outpPacket->m_sizeflag)
	{
		outpPacket->m_recvbytes += inCompletebyte;

		if (outpPacket->m_recvbytes == sizeof(PacketBase::psize_t))
		{
			memcpy(&outpPacket->m_target_recvbytes, outpPacket->m_buf, sizeof(PacketBase::psize_t));
			outpPacket->m_recvbytes = 0;
			outpPacket->m_sizeflag = false;
		}

		if (!PacketManager::RecvAsync(inpSock, outpPacket))
		{
			return E_PacketState::Error;
		}

		return E_PacketState::InComplete;
	}

	outpPacket->m_recvbytes += inCompletebyte;

	if (outpPacket->m_recvbytes != outpPacket->m_target_recvbytes)
	{
		if (!PacketManager::RecvAsync(inpSock, outpPacket))
		{
			return E_PacketState::Error;
		}
		return E_PacketState::InComplete;
	}
	return E_PacketState::Completed;
}

E_PacketState PacketManager::CompleteSend(TCPSocketPtr inpSock, SendPacketPtr inpPacket, const PacketBase::psize_t inCompletebyte)
{
	inpPacket->m_sendbytes += inCompletebyte;
	if (inpPacket->m_sendbytes != inpPacket->m_target_sendbytes)
	{
		if (!PacketManager::SendAsync(inpSock, inpPacket))
		{
			return E_PacketState::Error;
		}
		return E_PacketState::InComplete;
	}
	return E_PacketState::Completed;
}

DWORD __stdcall PacketManager::WorkerThread(LPVOID arg)
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

		if (client_info->GetState() == E_ClientState::Disconnected)
		{
			//RemoveClientInfo(ptr);
			continue;
		}


		E_PacketState result;

		switch (overlapped->type)
		{
		case  E_OverlappedType::Recv:
		{
			result = PacketManager::CompleteRecv(
				client_info->GetSockPtr(),
				std::static_pointer_cast<RecvPacket>(pPacket.lock()),
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

			// complete recv process

			if (!RecvAsync(client_info->GetSockPtr(), 
				std::static_pointer_cast<RecvPacket>(pPacket.lock())))
			{
				continue;
			}
		}
		break;

		case E_OverlappedType::Send:
		{
			result = PacketManager::CompleteSend(
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

		}
		break;
		}

	}

	return 0;
}
