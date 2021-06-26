#pragma once
class NetworkManager
{
public:
	static std::unique_ptr<NetworkManager> sInstance;
private:
	HandlePtr	 m_pHcp;			// iocp handle
	TCPSocketPtr m_pListenSock;
private:
	NetworkManager() {}
	bool Init(u_short inPort);
public:
	NetworkManager(const NetworkManager&) = delete;
	NetworkManager& operator=(const NetworkManager&) = delete;
	static bool StaticInit(u_short inPort);
	~NetworkManager();
public:
	bool DoFrame();
	HandlePtr GetHCPPtr() const;
	TCPSocketPtr GetListenSockPtr() const;
};

