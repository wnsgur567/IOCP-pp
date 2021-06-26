#pragma once

// 싱글톤으로 수정하기
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
	// iocp를 위한 스트림 풀링
	// 일반 동기화의 경우 소켓당 한개씩만 매칭 시킬 것
	std::queue<InputMemoryStreamPtr> m_inputstream_pool;
	std::queue<OutputMemoryStreamPtr> m_outputstream_pool;
private:
	unsigned __int32 m_curRecvPacketID;		// 최근에 받은 recv packet id
	unsigned __int32 m_newSendPacketID;		// 새로 만들 send packet id
public:	
	SendPacketPtr Packing(const void* inData, const size_t inSize);	// 받은 data 에 패킷id 와 size를 부여해서 패킹
	void UnPacking(const RecvPacketPtr inPacket, void* outData, size_t* outSize);

	E_PacketState RecvAsync();	// 비동기 recv
	E_PacketState SendAsync();	// 비동기 send
	bool PushSendQueue();		// sendqueue에등록
	E_PacketState CompleteRecv();
	E_PacketState CompleteSend();
};

