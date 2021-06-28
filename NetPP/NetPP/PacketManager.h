#pragma once

// �̱������� �����ϱ�
class PacketManager
{
public:
	static std::unique_ptr<PacketManager> sInstance;
	using psize_t = unsigned __int32;
	using id_t = unsigned __int32;
private:
	PacketManager()
		:m_curRecvPacketID(0), m_newSendPacketID(0)
	{}
	bool Init(psize_t inStreamPoolingCapacity);
public:
	PacketManager(const PacketManager&) = delete;
	PacketManager& operator=(const PacketManager&) = delete;
	static bool StaticInit();
	~PacketManager();

public:
	// iocp�� ���� ��Ʈ�� Ǯ��
	// �Ϲ� ����ȭ�� ��� ���ϴ� �Ѱ����� ��Ī ��ų ��
	std::queue<RecvPacketPtr> m_recvpacket_pool;
	std::queue<SendPacketPtr> m_sendpacket_pool;
private:
	unsigned __int32 m_curRecvPacketID;		// �ֱٿ� ���� recv packet id
	unsigned __int32 m_newSendPacketID;		// ���� ���� send packet id
public:
	//SendPacketPtr Packing(const void* inData, const size_t inSize);	// ���� data �� ��Ŷid �� size�� �ο��ؼ� ��ŷ
	//void UnPacking(const RecvPacketPtr inPacket, void* outData, size_t* outSize);


	void RequestSend(const TCPSocketPtr inpSock, SendPacketPtr inSendPacket);
	static bool RecvAsync(const TCPSocketPtr inpSock, RecvPacketPtr outRecvPacket);	// �񵿱� recv
	static bool SendAsync(const TCPSocketPtr inpSock, SendPacketPtr inSendPacket);	// �񵿱� send	
	static E_PacketState CompleteRecv(TCPSocketPtr inpSock, RecvPacketPtr outpPacket, const psize_t inCompletebyte);
	static E_PacketState CompleteSend(TCPSocketPtr inpSock, SendPacketPtr inpPacket, const psize_t inCompletebyte);
	static DWORD WINAPI WorkerThread(LPVOID arg);
};

