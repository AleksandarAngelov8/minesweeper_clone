#include "Client.h"
#include "Server.h"
#include "Scene/Scene.h"
#include "Button/ButtonFunctionality.h"
#include <iostream>

#define PORT 0
#define LOCAL_HOST "0.0.0.0"  // ZeroTier IP
#define SERVER_HOST "10.147.18.46"  // ZeroTier IP

void Client::SendToServer(const char* message)
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return;
    }

    // Create socket
    SOCKET clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);  // UDP socket
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << "\n";
        WSACleanup();
        return;
    }

    // Server address
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_HOST, &serverAddr.sin_addr);  // Use ZeroTier IP directly

    // Send data

    int sendResult = sendto(clientSocket, message, strlen(message), 0,
        (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (sendResult == SOCKET_ERROR) {
        //std::cerr << "sendto failed: " << WSAGetLastError() << "\n";
    }
    else {
        std::cout << "Input sent to server: " << message << "\n";
    }

    // Cleanup
    closesocket(clientSocket);
    WSACleanup();
}

void Client::Listening()
{
    //std::cout << "Server is now listening on ZeroTier IP: " << SERVER_HOST << ":" << PORT << "\n";

    // Receive data
    char buffer[1024] = { 0 };
    serverAddrSize = sizeof(serverAddr);

    while (true) {  // Loop to keep receiving packets
        int bytesReceived = recvfrom(Server::serverSocket, buffer, sizeof(buffer) - 1, 0,
            (sockaddr*)&serverAddr, &serverAddrSize);

        if (bytesReceived == SOCKET_ERROR) {
            //std::cerr << "Receive failed: " << WSAGetLastError() << "\n";
            break;
        }
        else {
            buffer[bytesReceived] = '\0';  // Null-terminate the string
            ButtonFunctionality::getCurrentSceneObject()->ReceiveInfo(buffer);

            inet_ntop(AF_INET, &serverAddr.sin_addr, serverIP, INET_ADDRSTRLEN);
            //std::cout << "Received from " << serverIP << ":" << ntohs(serverAddr.sin_port) << ": " << buffer << "\n";
        }
    }
}
sockaddr_in Client::serverAddr = {};
int Client::serverAddrSize = 0;
char serverIP[22] = {};