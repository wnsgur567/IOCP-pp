#include "base.h"

std::unique_ptr<PacketManager> PacketManager::sInstance;

bool PacketManager::Init(psize_t inStreamPoolingCapacity)
{
	for (psize_t i = 0; i < inStreamPoolingCapacity; i++)
	{
		// recvpacket
		RecvPacketPtr r_ptr = std::make_shared<RecvPacket>(BUFSIZE);
		r_ptr->Init(r_ptr);
		PacketManager::sInstance->m_recvpacket_pool.push(r_ptr);
		PacketManager::sInstance->m_recvpacket_container.push_back(r_ptr);
		// sendpacket
		SendPacketPtr s_ptr = std::make_shared<SendPacket>(BUFSIZE);
		s_ptr->Init(s_ptr);
		PacketManager::sInstance->m_sendpacket_pool.push(s_ptr);
		PacketManager::sInstance->m_sendpacket_container.push_back(s_ptr);
	}
	return true;
}

bool PacketManager::StaticInit()
{
	sInstance.reset(new PacketManager());

	// stream 을 capacity 만큼 생성 및 풀링
	return PacketManager::sInstance->Init(STREAMPOOLCAPACITY);
}

PacketManager::~PacketManager()
{

}

RecvPacketPtr PacketManager::GetRecvPacketFromPool()
{
	RecvPacketPtr pRetPacket = m_recvpacket_pool.front();
	m_recvpacket_pool.pop();
	pRetPacket->Clear();
	return pRetPacket;
}

void PacketManager::RetrieveRecvPacket(RecvPacketPtr inpPacket)
{
	m_recvpacket_pool.push(inpPacket);
}

SendPacketPtr PacketManager::GetSendPacketFromPool()
{
	SendPacketPtr pRetPacket = m_sendpacket_pool.front();
	m_sendpacket_pool.pop();
	pRetPacket->Clear();
	return pRetPacket;
}

void PacketManager::RetrieveSendPacket(SendPacketPtr inpPacket)
{
}
