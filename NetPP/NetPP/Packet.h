#pragma once
class Packet
{
};

// using input stream
class RecvPacket
{
private:
	unsigned __int32 m_capacity;
	char* m_buf;

	PacketManager::E_PacketState m_state;
	unsigned __int32		m_sizebytes;
	const unsigned __int32	m_target_sizebytes;
	unsigned __int32		m_databytes;
	unsigned __int32		m_target_databytes;

public:
	RecvPacket(unsigned __int32 inCapacity)
		: m_capacity(inCapacity),
		m_buf(nullptr),
		m_sizebytes(0),
		m_target_sizebytes(sizeof(unsigned __int32)),
		m_databytes(0),
		m_target_databytes(0)
	{
		m_buf = new char[m_capacity];
	}

	RecvPacket(const RecvPacket& _inSrc) :
		m_capacity(_inSrc.m_capacity),
		m_buf(nullptr),
		m_state(_inSrc.m_state),
		m_sizebytes(_inSrc.m_sizebytes),
		m_target_sizebytes(_inSrc.m_target_sizebytes),
		m_databytes(_inSrc.m_databytes),
		m_target_databytes(_inSrc.m_target_databytes)
	{
		m_buf = new char[m_capacity];
		memcpy(m_buf, _inSrc.m_buf, static_cast<size_t> (m_databytes));
	}

	~RecvPacket()
	{
		delete m_buf;
	}

	void Clear()
	{
		m_state = PacketManager::E_PacketState::Idle;
		m_sizebytes = 0;
		m_databytes = 0;
		m_target_databytes = 0;
		ZeroMemory(m_buf, m_capacity);
	}
	
	InputMemoryStreamPtr ToStreamPtr()
	{
		if (m_state != PacketManager::E_PacketState::Completed)
			return nullptr;
		return std::make_shared<InputMemoryStream>(m_buf, m_databytes);
	}
	// m_buf 를 memcpy 해서 스트림으로 반환
	InputMemoryStreamPtr ToCopyStreamPtr()
	{
		if (m_state != PacketManager::E_PacketState::Completed)
			return nullptr;
		char* tmpbuf = new char[m_databytes];
		memcpy(tmpbuf, m_buf, m_databytes);
		return std::make_shared<InputMemoryStream>(m_buf, m_databytes, true);
	}
};

// using output stream
class SendPacket
{
private:
};