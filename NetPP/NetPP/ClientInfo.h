#pragma once

class ClientInfo
{
private:
	TCPSocketPtr m_pSock;
	SocketAddress m_address;
private:
	unsigned __int32 m_id;
	E_ClientState m_state;
public:
	ClientInfo(TCPSocketPtr inpSock, SocketAddress inAddress);	
	void Init(unsigned __int32 inID);
public:
	TCPSocketPtr GetSockPtr() const;
	TCPSocketPtr GetSockPtr();
	SocketAddress GetSocketAddr() const;
	unsigned __int32 GetID() const;
	E_ClientState GetState() const;
};
