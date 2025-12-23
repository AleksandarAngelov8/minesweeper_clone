#pragma once
#include <iostream>
#include <winsock2.h>
#include <Ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

class Server
{
public: 
	static sockaddr_in clientAddr;
	static int clientAddrSize;
	static SOCKET serverSocket;
	static void SetupSocket();
	static void Listening();
	static void SendToClient(const char* message);
	static void CleanUp();
};
extern char clientIP[22];
