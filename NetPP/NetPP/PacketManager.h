#pragma once

// �̱������� �����ϱ�
class PacketManager
{
public:
	// iocp�� ���� ��Ʈ�� Ǯ��
	// �Ϲ� ����ȭ�� ��� ���ϴ� �Ѱ����� ��Ī ��ų ��
	std::queue<InputMemoryStreamPtr> m_inputstream_pool;
	std::queue<OutputMemoryStreamPtr> m_outputstream_pool;
private:
	unsigned __int32 m_curRecvPacketID;		// �ֱٿ� ���� recv packet id
	unsigned __int32 m_newSendPacketID;		// ���� ���� send packet id
public:
	PacketManager(unsigned __int32 inStreamPoolingCapacity);
	~PacketManager();

	void Initialize(unsigned __int32 inStreamPoolingCapacity);

	SendPacketPtr Packing(const void* inData, const size_t inSize);	// ���� data �� ��Ŷid �� size�� �ο��ؼ� ��ŷ
	void UnPacking(const RecvPacketPtr inPacket, void* outData, size_t* outSize);
};

