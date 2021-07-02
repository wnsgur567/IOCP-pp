#pragma once
class NetworkManager
{
protected:
	TCPSocketPtr m_pListenSock;
protected:
	NetworkManager() {}
	virtual bool Init(u_short inPort, bool isInNonBlock);
public:
	NetworkManager(const NetworkManager&) = delete;
	NetworkManager& operator=(const NetworkManager&) = delete;
	virtual ~NetworkManager();
public:
	virtual bool DoFrame() = 0;

	TCPSocketPtr GetListenSockPtr() const;
};

class ICOPNetworkManager : public NetworkManager
{
public:
	static std::unique_ptr<ICOPNetworkManager> sInstance;

	using psize_t = PacketManager::psize_t;
	using InitializeCallBack = bool(*)(void);
	using CompleteAcceptCallBack = bool (*)(void);
	using CompleteRecvCallBack = bool (*)(InputMemoryStreamPtr);
	using CompleteSendCallBack = bool (*)(OutputMemoryStreamPtr);
	using DisconnectedCallBack = void (*)(void);
public:
	// Init �Ϸ� �� 
	// ���⼭ ���� �Ŵ����� �ʱ�ȭ�մϴ�
	InitializeCallBack OnInit;
	// accept �Ϸ�� callback
	CompleteAcceptCallBack OnCompleteAccept;
	// recv �Ϸ�� callback
	CompleteRecvCallBack OnCompleteRecv;
	// send �Ϸ�� callback
	CompleteSendCallBack OnCompleteSend;
	// client ���� ���� �� callback
	DisconnectedCallBack OnDisconnected;
protected:
	HandlePtr	 m_pHcp;
	std::queue <std::pair<TCPSocketPtr, SendPacketPtr>> m_sendQueue;
protected:
	ICOPNetworkManager()
		:OnInit(), OnCompleteAccept(), OnCompleteRecv(), OnCompleteSend(), OnDisconnected() {}
	bool Init(u_short inPort, bool isInNonBlock) override;
public:
	static bool StaticInit(u_short inPort);
	ICOPNetworkManager(const ICOPNetworkManager&) = delete;
	ICOPNetworkManager& operator=(const ICOPNetworkManager&) = delete;
	~ICOPNetworkManager();
public:
	virtual bool DoFrame();
	HandlePtr GetHCPPtr() const;
public:
	bool PushSendQueue(TCPSocketPtr inpSock, SendPacketPtr inpSendPacket);		// sendqueue�����
	bool SendQueueProcess();		// send queue�� �ִ� ��Ŷ ó��

	static bool RecvAsync(const TCPSocketPtr inpSock, RecvPacketPtr& outRecvPacket);	// �񵿱� recv
	static bool SendAsync(const TCPSocketPtr inpSock, SendPacketPtr inSendPacket);	// �񵿱� send	
	static E_PacketState CompleteRecv(TCPSocketPtr inpSock, RecvPacketPtr& outpPacket, const psize_t inCompletebyte);
	static E_PacketState CompleteSend(TCPSocketPtr inpSock, SendPacketPtr inpPacket, const psize_t inCompletebyte);
	static DWORD WINAPI WorkerThread(LPVOID arg);
};

class SelectNetworkManager : public NetworkManager
{

};

