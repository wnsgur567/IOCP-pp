#pragma once
class NetworkManager
{
public:
	static std::unique_ptr<NetworkManager> sInstance;
protected:
	HandlePtr	 m_pHcp;			// iocp handle
	TCPSocketPtr m_pListenSock;
protected:
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
private:
	std::queue <std::pair<TCPSocketPtr, SendPacketPtr>> m_sendQueue;
public:
	bool PushSendQueue(TCPSocketPtr inpSock, SendPacketPtr inpSendPacket);		// sendqueue�����
	bool SendQueueProcess();		// send queue�� �ִ� ��Ŷ ó��
	static DWORD WINAPI AcceptThread(LPVOID arg);
};

