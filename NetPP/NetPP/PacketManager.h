#pragma once

// 싱글톤으로 수정하기
class PacketManager
{
public:
	static std::unique_ptr<PacketManager> sInstance;
	using psize_t = unsigned __int32;
	using id_t = unsigned __int32;

	using CompleteRecvCallBack = bool (*)(InputMemoryStreamPtr);
	using CompleteSendCallBack = bool (*)(OutputMemoryStreamPtr);

private:
	PacketManager()
		:m_curRecvPacketID(0), m_newSendPacketID(0),
		m_recvpacket_pool(), m_sendpacket_pool(),
		OnCompleteRecv(), OnCompleteSend()
	{}
	bool Init(psize_t inStreamPoolingCapacity);
public:
	PacketManager(const PacketManager&) = delete;
	PacketManager& operator=(const PacketManager&) = delete;
	static bool StaticInit();
	~PacketManager();

private:
	// 풀링한 패킷이 사라지지 않도록 hold 할 보관 컨테이너임...
	std::list<RecvPacketPtr> m_recvpacket_container;
	std::list<SendPacketPtr> m_sendpacket_container;
public:
	// iocp를 위한 스트림 풀링
	// 일반 동기화의 경우 소켓당 한개씩만 매칭 시킬 것
	std::queue<RecvPacketPtr> m_recvpacket_pool;
	std::queue<SendPacketPtr> m_sendpacket_pool;

	// recv 완료시 function pointer
	CompleteRecvCallBack OnCompleteRecv;
	// send 완료시 function pointer
	CompleteSendCallBack OnCompleteSend;
private:
	unsigned __int32 m_curRecvPacketID;		// 최근에 받은 recv packet id
	unsigned __int32 m_newSendPacketID;		// 새로 만들 send packet id
public:
	//SendPacketPtr Packing(const void* inData, const size_t inSize);	// 받은 data 에 패킷id 와 size를 부여해서 패킹
	//void UnPacking(const RecvPacketPtr inPacket, void* outData, size_t* outSize);


	RecvPacketPtr GetRecvPacketFromPool();
	void RetrieveRecvPacket(RecvPacketPtr inpPacket);
	SendPacketPtr GetSendPacketFromPool();
	void RetrieveSendPacket(SendPacketPtr inpPacket);
	
	void RequestSend(const TCPSocketPtr inpSock, SendPacketPtr inSendPacket);

	static bool RecvAsync(const TCPSocketPtr inpSock, RecvPacketPtr& outRecvPacket);	// 비동기 recv
	static bool SendAsync(const TCPSocketPtr inpSock, SendPacketPtr inSendPacket);	// 비동기 send	
	static E_PacketState CompleteRecv(TCPSocketPtr inpSock, RecvPacketPtr& outpPacket, const psize_t inCompletebyte);
	static E_PacketState CompleteSend(TCPSocketPtr inpSock, SendPacketPtr inpPacket, const psize_t inCompletebyte);
	static DWORD WINAPI WorkerThread(LPVOID arg);
};

