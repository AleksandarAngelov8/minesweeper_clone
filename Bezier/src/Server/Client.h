#pragma once

#include <winsock2.h>
#include <Ws2tcpip.h>

class Client
{
public:
	static sockaddr_in serverAddr;
	static int serverAddrSize;
	static void SendToServer(const char* message);
	static void Listening();
};
extern char serverIP[22];
