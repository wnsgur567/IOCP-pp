#pragma once

class ClientInfo
{
public:
	using id_t = ClientManager::id_t;
private:
	TCPSocketPtr m_pSock;
	SocketAddress m_address;
private:
	id_t m_id;
	E_ClientState m_state;
public:
	ClientInfo(TCPSocketPtr inpSock, SocketAddress inAddress);	
	void Init(id_t inID);
public:
	TCPSocketPtr GetSockPtr() const;
	TCPSocketPtr GetSockPtr();
	SocketAddress GetSocketAddr() const;
	id_t GetID() const;
	E_ClientState GetState() const;
	void SetState(E_ClientState inState);
};
