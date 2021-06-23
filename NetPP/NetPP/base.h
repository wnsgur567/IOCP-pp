#pragma once

#pragma comment(lib,"ws2_32")
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<iostream>
#include<vector>
#include<list>
#include<map>
#include<algorithm>

constexpr u_short SERVERPORT = 9000;
constexpr unsigned int BUFSIZE = 512;
#define SOCKET_END	0


class TCPSocket;

using HandlePtr = std::shared_ptr<HANDLE>;
using TCPSocketPtr = std::shared_ptr<TCPSocket>;

#include "CriticalSection.h"

#include "SocketUtil.h"
#include "SocketAddress.h"
#include "TCPSocket.h"