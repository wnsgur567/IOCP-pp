#pragma once

#pragma comment(lib,"ws2_32")
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<iostream>
#include<vector>
#include<list>
#include<map>
#include<algorithm>
#undef max

constexpr u_short			SERVERPORT	= 9000;
constexpr unsigned __int32	BUFSIZE		= 512;
#define SOCKET_END	0

class TCPSocket;
class InputMemoryStream;
class OutputMemoryStream;
class RecvPacket;
class SendPacket;

using HandlePtr = std::shared_ptr<HANDLE>;
using TCPSocketPtr = std::shared_ptr<TCPSocket>;
using InputMemoryStreamPtr = std::shared_ptr<InputMemoryStream>;
using OutputMemoryStreamPtr = std::shared_ptr<OutputMemoryStream>;
using RecvPacketPtr = std::shared_ptr<RecvPacket>();
using SendPacketPtr = std::shared_ptr<SendPacket>();

#include "CriticalSection.h"

#include "MemoryStream.h"
#include "SocketUtil.h"
#include "SocketAddress.h"
#include "TCPSocket.h"

#include "Packet.h"
#include "PacketManager.h"