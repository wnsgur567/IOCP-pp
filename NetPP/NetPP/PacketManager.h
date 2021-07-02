#pragma once

// �̱������� �����ϱ�
class PacketManager
{
	friend class ICOPNetworkManager;
public:
	static std::unique_ptr<PacketManager> sInstance;
	using psize_t = unsigned __int32;
	using id_t = unsigned __int32;	

private:
	PacketManager()
		:m_curRecvPacketID(0), m_newSendPacketID(0),
		m_recvpacket_pool(), m_sendpacket_pool()		
	{}
	bool Init(psize_t inStreamPoolingCapacity);
public:
	PacketManager(const PacketManager&) = delete;
	PacketManager& operator=(const PacketManager&) = delete;
	static bool StaticInit();
	~PacketManager();

private:
	// Ǯ���� ��Ŷ�� ������� �ʵ��� hold �� ���� �����̳���...
	std::list<RecvPacketPtr> m_recvpacket_container;
	std::list<SendPacketPtr> m_sendpacket_container;
public:
	// iocp�� ���� ��Ʈ�� Ǯ��
	// �Ϲ� ����ȭ�� ��� ���ϴ� �Ѱ����� ��Ī ��ų ��
	std::queue<RecvPacketPtr> m_recvpacket_pool;
	std::queue<SendPacketPtr> m_sendpacket_pool;
	
private:
	// TODO : �� Ŭ���̾�Ʈ ���� �����ϰ� �ٲ���
	unsigned __int32 m_curRecvPacketID;		// �ֱٿ� ���� recv packet id
	unsigned __int32 m_newSendPacketID;		// ���� ���� send packet id
public:
	RecvPacketPtr GetRecvPacketFromPool();
	void RetrieveRecvPacket(RecvPacketPtr inpPacket);
	SendPacketPtr GetSendPacketFromPool();
	void RetrieveSendPacket(SendPacketPtr inpPacket);
};

// session ���� ��
//SendPacketPtr Packing(const void* inData, const size_t inSize);	// ���� data �� ��Ŷid �� size�� �ο��ؼ� ��ŷ
//void UnPacking(const RecvPacketPtr inPacket, void* outData, size_t* outSize);