#include "base.h"

RecvPacket::RecvPacket(psize_t inStreamCapacity)
	: PacketBase(E_OverlappedType::Recv, inStreamCapacity + sizeof(psize_t) + sizeof(id_t)),
	m_pStream(nullptr),
	m_sizeflag(true),
	m_recvbytes(0),
	m_target_recvbytes(0)
{
}

RecvPacket::RecvPacket(InputMemoryStreamPtr inStreamPtr)
	: PacketBase(E_OverlappedType::Recv, inStreamPtr->GetCapacity() + sizeof(psize_t) + sizeof(id_t)),
	m_pStream(inStreamPtr),
	m_sizeflag(true),
	m_recvbytes(0),
	m_target_recvbytes(0)
{

}

RecvPacket::~RecvPacket()
{

}

void RecvPacket::GetReady()
{
	m_overlappedEx.flush();
	m_wsabuf.buf = m_pStream->m_buffer + m_recvbytes;
	if (m_sizeflag)
	{
		m_wsabuf.len = sizeof(psize_t) - m_recvbytes;
	}
	else
	{
		m_wsabuf.len = m_target_recvbytes - m_recvbytes;
	}
}

void RecvPacket::Init(RecvPacketPtr inpThis)
{
	m_overlappedEx.pPacket = inpThis;
}

void RecvPacket::Clear()
{
	m_sizeflag = true;
	m_recvbytes = 0;
	m_target_recvbytes = 0;
}


SendPacket::SendPacket(psize_t inStreamCapacity)
	:PacketBase(E_OverlappedType::Send, inStreamCapacity + sizeof(psize_t) + sizeof(id_t)),
	m_pStream(nullptr),
	m_sendbytes(0),
	m_target_sendbytes(0)

{
}

SendPacket::SendPacket(OutputMemoryStreamPtr inStreamPtr)
	: PacketBase(E_OverlappedType::Send, inStreamPtr->GetCapacity() + sizeof(psize_t) + sizeof(id_t)),
	m_pStream(inStreamPtr),
	m_sendbytes(0),
	m_target_sendbytes(0)
{

}

void SendPacket::GetReady(const id_t inPacketID)
{
	// 패킷 구성 : total packet size + packet id + stream

	m_overlappedEx.flush();
	if (m_state == E_PacketState::Idle)
	{	// 처음 Send 하는 경우
		// total packet size = packet id size + stream size
		psize_t total_size = sizeof(id_t) + m_pStream->GetLength();
		memcpy(m_buf, &total_size, sizeof(psize_t));
		memcpy(m_buf + sizeof(psize_t), &inPacketID, sizeof(id_t));
		memcpy(m_buf + sizeof(psize_t) + sizeof(id_t), m_pStream->GetBufferPtr(), m_pStream->GetLength());
		m_target_sendbytes = total_size + sizeof(psize_t);
	}

	m_wsabuf.buf = m_buf + m_sendbytes;
	m_wsabuf.len = m_target_sendbytes - m_sendbytes;
}

void SendPacket::Init(SendPacketPtr inpThis)
{
	m_overlappedEx.pPacket = inpThis;
}

void SendPacket::Clear()
{
	m_state = E_PacketState::Idle;
	m_sendbytes = 0;
	m_target_sendbytes = 0;
}

