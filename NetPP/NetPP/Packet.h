#pragma once


// 상속하면 안됨
// vtable 생기면 workerthread 에 문제 생길 수 있음
// ps :: weak ptr -> std::enable_shared_from_this<> 를 상속하여 shared 로 구현 가능하지만 vtable 이 생길텐데...
// shared 사용시 위에 class 상속 받으면 this 참조 스마트 포인터라도 정상적으로 동작함
// 일단 enable_shared_from_this 는 virtual 없어보임
struct OverlappedEx // : std::enable_shared_from_this<OverlappedEx> 
{
	WSAOVERLAPPED overlapped;
	E_OverlappedType type;
	PacketBaseWeakPtr pPacket;
	void* ptr;

	OverlappedEx(E_OverlappedType inType)
		: overlapped(), type(inType)
		, ptr(nullptr)
	{

	}
	void Init(PacketBasePtr inpPacket)
	{
		pPacket = inpPacket;
	}
	void flush()
	{
		ZeroMemory(&overlapped, sizeof(overlapped));
	}
};

class PacketBase
{
public:
	using psize_t = PacketManager::psize_t;
	using id_t = PacketManager::id_t;
protected:
	id_t m_id;
	E_PacketState m_state;

	OverlappedEx m_overlappedEx;
	WSABUF		 m_wsabuf;
	psize_t		 m_capacity;
	Byte* m_buf;
public:
	unsigned __int32 GetId() const { return m_id; }
	void SetId(unsigned __int32 inId) { m_id = inId; }

	virtual void Init(PacketBasePtr) = 0;
	virtual void Clear() = 0;
protected:
	PacketBase(E_OverlappedType inType, const psize_t inCapacity) :
		m_id(0), m_state(E_PacketState::Idle),
		m_overlappedEx(inType),
		m_wsabuf(),
		m_capacity(inCapacity),
		m_buf(nullptr)
	{
		if (m_capacity > 0)
			m_buf = new Byte[m_capacity];
	}
	virtual ~PacketBase()
	{
		if (m_buf != nullptr)
			delete[] m_buf;
	}
};

class AcceptPacket : public PacketBase
{
	friend class PacketManager;
	friend class IOCPNetworkManager;
private:
	TCPSocketPtr m_pClientSock;
	SocketAddress m_sockAddr;

public:
	AcceptPacket()
		:PacketBase(E_OverlappedType::Accept, 0)
	{

	}
	AcceptPacket(TCPSocketPtr inSock, SocketAddress inAddr)
		:PacketBase(E_OverlappedType::Accept, 0),
		m_pClientSock(inSock), m_sockAddr(inAddr)
	{

	}

	void Init(PacketBasePtr inpPacket) override;
	void GetReady();

	TCPSocketPtr GetPSock();
	SocketAddress GetAddr();
	void Clear() override;
};


// using input stream
class RecvPacket : public PacketBase
{
	friend class PacketManager;
	friend class IOCPNetworkManager;
public:
	using time_point_t = std::chrono::time_point<std::chrono::high_resolution_clock>;

private:
	InputMemoryStreamPtr m_pStream;	// session 에서 사용할 datastream

	bool		m_sizeflag;
	psize_t		m_recvbytes;
	psize_t		m_target_recvbytes;

	time_point_t m_recv_time;	// 패킷 수신이 완료된 시간 complete recv
public:
	RecvPacket(psize_t inStreamCapacity);
	RecvPacket(InputMemoryStreamPtr inStreamPtr);
	~RecvPacket();

	void Init(PacketBasePtr inpThis);
	// packet을 get 하여 처음 사용하기 시작할때 호출 됨
	void Clear() override;

	// recv 전 overlapped 및 wsabuf 초기화
	void GetReady();
	InputMemoryStreamPtr GetStream();

	// 패킷 recv 가 완료된 시간을 기록
	void RecordRecvTime();
	time_point_t GetRecvTime() const;
};

// using output stream
class SendPacket : public PacketBase
{
	friend class PacketManager;
	friend class IOCPNetworkManager;
private:
	OutputMemoryStreamPtr m_pStream; // session 에서 사용할 datastream

	psize_t		m_sendbytes;				// 현재 send 수치
	psize_t		m_target_sendbytes;			// 목표 send 수치
public:
	SendPacket(psize_t inStreamCapacity);
	SendPacket(OutputMemoryStreamPtr inStreamPtr);

	void Init(PacketBasePtr inpThis) override;
	// packet을 get 하여 처음 사용하기 시작할때 호출 됨
	void Clear() override;

	// recv 전 overlapped 및 wsabuf 초기화
	void GetReady(const id_t inPacketID);
};