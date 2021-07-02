#pragma once



struct OverlappedEx
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
	friend class ICOPNetworkManager;
private:
	TCPSocketPtr m_pClientSock;
	SocketAddress m_sockAddr;

public:
	AcceptPacket(TCPSocketPtr inSock, SocketAddress inAddr)
		:PacketBase(E_OverlappedType::Accept, 0),
		m_pClientSock(inSock), m_sockAddr(inAddr)
	{

	}

	TCPSocketPtr GetPSock();
	SocketAddress GetAddr();
	void Clear() override;
};


// using input stream
class RecvPacket : public PacketBase
{
	friend class PacketManager;
	friend class ICOPNetworkManager;
public:
	using time_point_t = std::chrono::time_point<std::chrono::high_resolution_clock>;

private:
	InputMemoryStreamPtr m_pStream;	// session ���� ����� datastream

	bool		m_sizeflag;
	psize_t		m_recvbytes;
	psize_t		m_target_recvbytes;

	time_point_t m_recv_time;	// ��Ŷ ������ �Ϸ�� �ð� complete recv
public:
	RecvPacket(psize_t inStreamCapacity);
	RecvPacket(InputMemoryStreamPtr inStreamPtr);
	~RecvPacket();

	void Init(RecvPacketPtr inpThis);
	// packet�� get �Ͽ� ó�� ����ϱ� �����Ҷ� ȣ�� ��
	void Clear() override;

	// recv �� overlapped �� wsabuf �ʱ�ȭ
	void GetReady();
	InputMemoryStreamPtr GetStream();

	// ��Ŷ recv �� �Ϸ�� �ð��� ���
	void RecordRecvTime();
	time_point_t GetRecvTime() const;
};

// using output stream
class SendPacket : public PacketBase
{
	friend class PacketManager;
	friend class ICOPNetworkManager;
private:
	OutputMemoryStreamPtr m_pStream; // session ���� ����� datastream

	psize_t		m_sendbytes;				// ���� send ��ġ
	psize_t		m_target_sendbytes;			// ��ǥ send ��ġ
public:
	SendPacket(psize_t inStreamCapacity);
	SendPacket(OutputMemoryStreamPtr inStreamPtr);

	void Init(SendPacketPtr inpThis);
	// packet�� get �Ͽ� ó�� ����ϱ� �����Ҷ� ȣ�� ��
	void Clear() override;

	// recv �� overlapped �� wsabuf �ʱ�ȭ
	void GetReady(const id_t inPacketID);
};