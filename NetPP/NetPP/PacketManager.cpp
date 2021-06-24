#include "base.h"



PacketManager::PacketManager(unsigned __int32 inStreamPoolingCapacity)
	: m_curRecvPacketID(0), m_newSendPacketID(0)
{
	Initialize(inStreamPoolingCapacity);
}

PacketManager::~PacketManager()
{
}

// stream 을 capacity 만큼 생성 및 풀링
void PacketManager::Initialize(unsigned __int32 inStreamPoolingCapacity)
{
	for (unsigned __int32 i = 0; i < inStreamPoolingCapacity; i++)
	{
		m_inputstream_pool.push(std::make_shared<InputMemoryStream>(BUFSIZE));
		m_outputstream_pool.push(std::make_shared<OutputMemoryStream>(BUFSIZE));
	}
}

SendPacketPtr PacketManager::Packing(const void* inData, const size_t inSize)
{
	


	return nullptr;
}

void PacketManager::UnPacking(const RecvPacketPtr inPacket, void* outData, size_t* outSize)
{
}
