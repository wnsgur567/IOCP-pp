#include "base.h"

RecvPacket::RecvPacket(InputMemoryStreamPtr inStreamPtr)
	: PacketBase(E_OverlappedType::Recv),
	m_pStream(inStreamPtr),
	m_sizebytes(0),
	m_target_sizebytes(sizeof(unsigned __int32)),
	m_databytes(0),
	m_target_databytes(0)
{
	
}

RecvPacket::~RecvPacket()
{
	
}

void RecvPacket::Clear()
{
	m_sizebytes = 0;
	m_databytes = 0;
	m_target_databytes = 0;	
}


SendPacket::SendPacket(OutputMemoryStreamPtr inStreamPtr)
	: PacketBase(E_OverlappedType::Send),
	m_pStream(inStreamPtr),
	m_sendbytes(0),
	m_target_sendbytes(0)
{

}

void SendPacket::Clear()
{
	m_state = E_PacketState::Idle;
	m_sendbytes = 0;
	m_target_sendbytes = 0;
}
