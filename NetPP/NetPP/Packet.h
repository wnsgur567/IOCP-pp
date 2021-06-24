#pragma once



struct OverlappedEx
{
	WSAOVERLAPPED overlapped;
	E_OverlappedType type;

	OverlappedEx(E_OverlappedType inType)
		: type(inType)
	{

	}
};

class PacketBase
{
protected:
	unsigned __int32 m_id;
	E_PacketState m_state;

	OverlappedEx m_overlappedEx;
	WSABUF m_wsabuf;
public:
	unsigned __int32 GetId() const { return m_id; }
	void SetId(unsigned __int32 inId) { m_id = inId; }
protected:
	PacketBase(E_OverlappedType inType) : 
		m_id(0), m_state(E_PacketState::Idle), 
		m_overlappedEx(inType), m_wsabuf(){}
	~PacketBase() {}
};


// using input stream
class RecvPacket : public PacketBase
{
private:
	InputMemoryStreamPtr m_pStream;
	
	unsigned __int32		m_sizebytes;
	const unsigned __int32	m_target_sizebytes;
	unsigned __int32		m_databytes;
	unsigned __int32		m_target_databytes;
public:
	RecvPacket(InputMemoryStreamPtr inStreamPtr);
	~RecvPacket();

	void Clear();	
};

// using output stream
class SendPacket : public PacketBase
{
private:
	OutputMemoryStreamPtr m_pStream;
	
	unsigned __int32		m_sendbytes;				// 현재 send 수치
	unsigned __int32		m_target_sendbytes;			// 목표 send 수치
public:
	SendPacket(OutputMemoryStreamPtr inStreamPtr);
	
	void Clear();
};