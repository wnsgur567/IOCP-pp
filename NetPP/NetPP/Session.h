#pragma once

template<typename T>
class Session : public Singleton<T>
{
public:
	enum class E_SessionState
	{
		BeforeInitialize,
		Initialized,
		Running,
		Finalized,
	};
protected:
	CriticalSection m_cs;
	E_SessionState m_state;
public:
	CriticalSection* GetCSPtr()
	{
		return &m_cs;
	}
	E_SessionState GetState() const
	{
		return m_state;
	}
protected:
	Session() : m_state(E_SessionState::BeforeInitialize) { }
protected:
	virtual bool StaticInitialize() = 0;
public:
	virtual bool Initialize()
	{
		m_state = E_SessionState::Initialized;
		return true;
	}
	virtual void Finalize()
	{
		m_state = E_SessionState::Finalized;
	}
	virtual ~Session() { }
public:
	virtual bool MainLoop() = 0;
};

class IOCPSession : public Session<IOCPSession>
{
	friend class Singleton;
protected:
	IOCPSession();
protected:
	bool StaticInitialize() override;
public:
	bool Initialize() override;
	void Finalize() override;
	~IOCPSession();
public:
	bool MainLoop() override;

	void Recv(HANDLE hEvent);
	void Send();
public:
	// call back ±¸Çö
	static bool OnAccept(TCPSocketPtr, SocketAddress);
	static bool OnCompleteRecv(InputMemoryStreamPtr);
	static bool OnCompleteSend(OutputMemoryStreamPtr);
	static void OnDisconnected(ClientInfoPtr);
};