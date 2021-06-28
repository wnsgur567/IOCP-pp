#include "base.h"

ClientInfo::ClientInfo(TCPSocketPtr inpSock, SocketAddress inAddress)
	: m_pSock(inpSock),
	m_address(inAddress),
	m_id(-1),
	m_state(E_ClientState::None)
{
}

void ClientInfo::Init(id_t inID)
{
	m_id = inID;
}

TCPSocketPtr ClientInfo::GetSockPtr() const
{
	return m_pSock;
}
TCPSocketPtr ClientInfo::GetSockPtr()
{
	return m_pSock;
}

SocketAddress ClientInfo::GetSocketAddr() const
{
	return m_address;
}

ClientInfo::id_t ClientInfo::GetID() const
{
	return m_id;
}

E_ClientState ClientInfo::GetState() const
{
	return m_state;
}

void ClientInfo::SetState(E_ClientState inState)
{
	m_state = inState;
}
