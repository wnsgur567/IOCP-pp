#pragma once

// �̱������� �����ϱ�
class PacketManager
{
public:
	static std::unique_ptr<PacketManager> sInstance;

private:
	PacketManager()
		:m_curRecvPacketID(0), m_newSendPacketID(0)
	{}
	bool Init(unsigned __int32 inStreamPoolingCapacity);
public:
	PacketManager(const PacketManager&) = delete;
	PacketManager& operator=(const PacketManager&) = delete;
	static bool StaticInit();
	~PacketManager();

public:
	// iocp�� ���� ��Ʈ�� Ǯ��
	// �Ϲ� ����ȭ�� ��� ���ϴ� �Ѱ����� ��Ī ��ų ��
	std::queue<InputMemoryStreamPtr> m_inputstream_pool;
	std::queue<OutputMemoryStreamPtr> m_outputstream_pool;
private:
	unsigned __int32 m_curRecvPacketID;		// �ֱٿ� ���� recv packet id
	unsigned __int32 m_newSendPacketID;		// ���� ���� send packet id
public:	
	SendPacketPtr Packing(const void* inData, const size_t inSize);	// ���� data �� ��Ŷid �� size�� �ο��ؼ� ��ŷ
	void UnPacking(const RecvPacketPtr inPacket, void* outData, size_t* outSize);

	E_PacketState RecvAsync();	// �񵿱� recv
	E_PacketState SendAsync();	// �񵿱� send
	bool PushSendQueue();		// sendqueue�����
	E_PacketState CompleteRecv();
	E_PacketState CompleteSend();
};

