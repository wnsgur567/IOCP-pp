#include "base.h"

std::unique_ptr<PacketManager> PacketManager::sInstance;

bool PacketManager::Init(unsigned __int32 inStreamPoolingCapacity)
{
	for (unsigned __int32 i = 0; i < inStreamPoolingCapacity; i++)
	{
		m_inputstream_pool.push(std::make_shared<InputMemoryStream>(BUFSIZE));
		m_outputstream_pool.push(std::make_shared<OutputMemoryStream>(BUFSIZE));
	}
	return true;
}

bool PacketManager::StaticInit()
{
	// stream �� capacity ��ŭ ���� �� Ǯ��
	return PacketManager::sInstance->Init(STREAMPOOLCAPACITY);
}

PacketManager::~PacketManager()
{
}




SendPacketPtr PacketManager::Packing(const void* inData, const size_t inSize)
{
	


	return nullptr;
}

void PacketManager::UnPacking(const RecvPacketPtr inPacket, void* outData, size_t* outSize)
{


}
