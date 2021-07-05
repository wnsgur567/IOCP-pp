#pragma once

enum class E_ManagerType
{
	None,
	Sign,
	Game,
};


class BaseManager
{
protected:
	HANDLE hRecvEvent;
	std::queue<std::pair<ClientInfoPtr, InputMemoryStreamPtr>> m_recvQueue;
	virtual void OnRecved(ClientInfoPtr inpInfo, InputMemoryStreamPtr inpStream)
	{
		m_recvQueue.push({ inpInfo, inpStream });
	}

	std::queue<std::pair<TCPSocketPtr, OutputMemoryStreamPtr>> m_sendQueue;
	virtual void Send(TCPSocketPtr inpSock, OutputMemoryStreamPtr inpStream)
	{
		m_sendQueue.push({ inpSock,inpStream });
		if (m_sendQueue.size() > 1)
			return;

		// temp
		IOCPSession::sInstance->Send();
	}
	virtual void OnSended()
	{
		m_sendQueue.pop();
		if (m_sendQueue.empty())
			return;

		// temp
		IOCPSession::sInstance->Send();
	}
	virtual bool DoFrame() = 0;
public:
	BaseManager()
	{
		hRecvEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	}
	void SetRecvEvent()
	{
		SetEvent(hRecvEvent);
	}

	static DWORD WINAPI ProcessThread(PVOID arg);
};