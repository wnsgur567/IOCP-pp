#pragma once

#pragma comment(lib,"ws2_32")
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<iostream>
#include<vector>
#include<list>
#include<unordered_map>
#include<unordered_set>
#include<map>
#include<set>
#include<algorithm>
#include<stack>
#include<queue>
#include<chrono>

#undef max
using Byte = char;

constexpr u_short			SERVERPORT = 9000;
constexpr unsigned __int32	BUFSIZE = 512;
constexpr unsigned __int32	STREAMPOOLCAPACITY = 128;
#define SOCKET_END	0

class TCPSocket;
class InputMemoryStream;
class OutputMemoryStream;
class AcceptPacket;
class RecvPacket;
class SendPacket;
class ClientInfo;
class PacketBase;
class IOCPNetworkManager;

using HandlePtr = std::shared_ptr<HANDLE>;
using TCPSocketPtr = std::shared_ptr<TCPSocket>;
using InputMemoryStreamPtr = std::shared_ptr<InputMemoryStream>;
using OutputMemoryStreamPtr = std::shared_ptr<OutputMemoryStream>;
using AcceptPacketPtr = std::shared_ptr<AcceptPacket>;
using RecvPacketPtr = std::shared_ptr<RecvPacket>;
using SendPacketPtr = std::shared_ptr<SendPacket>;
using ClientInfoPtr = std::shared_ptr<ClientInfo>;
using PacketBasePtr = std::shared_ptr<PacketBase>;
using PacketBaseWeakPtr = std::weak_ptr<PacketBase>;

enum class E_PacketState
{
	Error = -2,		// ������ ����
	End = -1,		// ���� ����

	Idle = 0,
	InComplete,		// �̿ϼ�, ������ �޴�(����) ��
	Completed,		// �ϼ�
};

enum class E_OverlappedType
{
	Accept,
	Recv,
	Send,
};

enum class E_SessionType
{
	None,

	Sign,

};

enum class E_ClientState
{
	None,

	Disconnected,
};

// core
#include "Singleton.h"
#include "CriticalSection.h"

// net core & client 
#include "MemoryStream.h"
#include "SocketUtil.h"
#include "SocketAddress.h"
#include "TCPSocket.h"
#include "ClientManager.h"
#include "ClientInfo.h"
#include "PacketManager.h"
#include "Packet.h"

#include "NetworkManager.h"


//////////////////////////////////

#include "ApplicationLevelTest.h"
