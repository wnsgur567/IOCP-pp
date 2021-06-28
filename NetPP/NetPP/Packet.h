#pragma once



struct OverlappedEx
{
	WSAOVERLAPPED overlapped;
	E_OverlappedType type;
	PacketBaseWeakPtr pPacket;

	OverlappedEx(E_OverlappedType inType)
		: overlapped(), type(inType)
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
	char* m_buf;
	psize_t		 m_capacity;
public:
	unsigned __int32 GetId() const { return m_id; }
	void SetId(unsigned __int32 inId) { m_id = inId; }

	virtual void Clear() = 0;
protected:
	PacketBase(E_OverlappedType inType, const psize_t inCapacity) :
		m_id(0), m_state(E_PacketState::Idle),
		m_overlappedEx(inType),
		m_wsabuf(),
		m_buf(nullptr), m_capacity(inCapacity)
	{
		m_buf = new char[m_capacity];
	}
	virtual ~PacketBase()
	{
		delete[] m_buf;
	}
};


// using input stream
class RecvPacket : public PacketBase
{
	friend class PacketManager;
private:
	InputMemoryStreamPtr m_pStream;

	bool		m_sizeflag;
	psize_t		m_recvbytes;
	psize_t		m_target_recvbytes;
public:
	RecvPacket(psize_t inStreamCapacity);
	RecvPacket(InputMemoryStreamPtr inStreamPtr);
	~RecvPacket();
	
	void Init(RecvPacketPtr inpThis);
	void Clear() override;
	
	// recv �� overlapped �� wsabuf �ʱ�ȭ
	void GetReady();
};

// using output stream
class SendPacket : public PacketBase
{
	friend class PacketManager;
private:
	OutputMemoryStreamPtr m_pStream;

	psize_t		m_sendbytes;				// ���� send ��ġ
	psize_t		m_target_sendbytes;			// ��ǥ send ��ġ
public:
	SendPacket(psize_t inStreamCapacity);
	SendPacket(OutputMemoryStreamPtr inStreamPtr);

	void Init(SendPacketPtr inpThis);
	void Clear() override;

	// recv �� overlapped �� wsabuf �ʱ�ȭ
	void GetReady(const id_t inPacketID);
};