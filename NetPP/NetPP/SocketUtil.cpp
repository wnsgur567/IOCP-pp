#include "base.h"

bool SocketUtil::Init()
{
	WSADATA _wsa;
	if (WSAStartup(MAKEWORD(2, 2), &_wsa) != 0)
	{
		ReportError("SocketUtil::Init");
		return false;
	}
	return true;
}
void SocketUtil::CleanUp()
{
	WSACleanup();
}

int SocketUtil::GetLastError()
{
	return WSAGetLastError();
}

void SocketUtil::ReportError(const char* inOperationDesc)
{
	LPVOID lpMsgBuf;
	DWORD errorNum = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		errorNum,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	printf("Error %s: %d- %s", inOperationDesc, errorNum, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

TCPSocketPtr SocketUtil::CreateTCPSocket()
{
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == SOCKET_ERROR)
	{
		ReportError("SocketUtil::CreateTCPSocket");
		return nullptr;
	}
	return std::make_shared<TCPSocket>(sock);
}

// 
HandlePtr SocketUtil::CreateIOCP(LPTHREAD_START_ROUTINE inWorkThreadPtr, std::vector<HandlePtr>& outWorkerThreads)
{
	HandlePtr hcpPtr;
	hcpPtr = std::make_shared<HANDLE>(
		CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0));
	if (*hcpPtr == nullptr)
		return nullptr;

	// CPU ���� Ȯ��
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	// (CPU ���� * 2)���� �۾��� ������ ����
	HANDLE hThread;
	for (int i = 0; i < (int)si.dwNumberOfProcessors * 2; i++) {
		hThread = CreateThread(NULL, 0, inWorkThreadPtr, *hcpPtr, 0, NULL);
		if (hThread == NULL)
			return nullptr;
		outWorkerThreads.push_back(std::make_shared<HANDLE>(hThread));
		CloseHandle(hThread);
	}

	return hcpPtr;
}

HandlePtr SocketUtil::LinkIOCPThread(ClientInfoPtr inInfo)
{
	return std::make_shared<HANDLE>(
		CreateIoCompletionPort((HANDLE)inInfo->GetSockPtr()->GetSock(),
			*IOCPNetworkManager::sInstance->GetHCPPtr(),
			inInfo->GetID(),
			0));
}

int SocketUtil::Select(
	const std::vector< TCPSocketPtr >* inReadVec,	//	recv�� �����ؾ��� socket��
	std::vector< TCPSocketPtr >* outReadVec,			//	select �� ���� �� socket��
	const std::vector< TCPSocketPtr >* inWriteVec,	//	send�� �����ؾ��� socket��
	std::vector< TCPSocketPtr >* outWriteVec			//	select �� ���� �� socket��
	/*const vector< TCPSocketPtr >* inExceptSet,
	vector< TCPSocketPtr >* outExceptSet*/
)
{
	fd_set read_set;
	fd_set write_set;
	//fd_set except_set;	

	FD_ZERO(&read_set);
	FD_ZERO(&write_set);
	//FD_ZERO(&except_set);

	// �ش� ���Ϳ� �����ϴ� ���ϵ��� �� set�� �����
	fd_set* readPtr = FillSetFromVector(read_set, inReadVec);
	fd_set* writePtr = FillSetFromVector(write_set, inWriteVec);
	//fd_set* exceptPtr = FillSetFromVector(except_set, inExceptSet);

	// select()
	int retval = select(0, readPtr, writePtr, nullptr, nullptr);

	if (retval == SOCKET_ERROR)
	{
		return false;
	}


	// �ش� set�� �����ִ� ���ϵ��� ���ͷ� ����
	FillVectorFromSet(outReadVec, inReadVec, read_set);
	FillVectorFromSet(outWriteVec, inWriteVec, write_set);
	//FillVectorFromSet(outExceptSet, inExceptSet, except_set);

	return true;
}

// socket���� outSet�� �����
fd_set* SocketUtil::FillSetFromVector(
	fd_set& outSet,		// ����� set (read or write)
	const std::vector<TCPSocketPtr>* inSockets	// set�� ���� socket vec
)
{
	if (inSockets)
	{	// not null
		FD_ZERO(&outSet);
		// ��� socket�� ���
		for (const TCPSocketPtr& socket : *inSockets)
		{
			FD_SET(socket->GetSock(), &outSet);
		}
		return &outSet;
	}
	return nullptr;
}

// inSet �� �˻��ؼ� �����ִ� ���ϵ��� �˻�
// inSockets : ó�� �����Ϸ��� ���� ���ϵ��� ISSET ���� �˻��Ͽ� 
// �����ִ� ���ϵ��� outSokcets�� ���
void SocketUtil::FillVectorFromSet(
	std::vector< TCPSocketPtr >* outSockets,
	const std::vector< TCPSocketPtr >* inSockets,
	const fd_set& inSet
)
{
	if (inSockets && outSockets)
	{
		outSockets->clear();
		for (const TCPSocketPtr& socket : *inSockets)
		{
			if (FD_ISSET(socket->GetSock(), &inSet))
			{
				outSockets->push_back(socket);
			}
		}
	}
}