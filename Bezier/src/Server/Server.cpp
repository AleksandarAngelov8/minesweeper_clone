#include "Server.h"
#include "Button/ButtonFunctionality.h"
#include "Scene/Scene.h"

#define PORT 8080
#define SERVER_HOST "0.0.0.0"  // ZeroTier IP
#define ZEROTIER_IP "10.147.18.46"

void Server::SetupSocket()
{
    WSADATA wsaData;
    std::cout << "Initializing Winsock...\n";
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return;
    }

    // Create socket
    serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    std::cout << "Creating socket...\n";
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << "\n";
        WSACleanup();
        return;
    }

    // Server address
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_HOST, &serverAddr.sin_addr);  // Bind to ZeroTier IP

    // Bind socket
    std::cout << "Binding to IP: " << SERVER_HOST << " and port " << PORT << "...\n";
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed: " << WSAGetLastError() << "\n";
        closesocket(serverSocket);
        WSACleanup();
    }
}

void Server::Listening()
{
    std::cout << "Server is now listening on ZeroTier IP: " << SERVER_HOST << ":" << PORT << "\n";

    // Receive data
    char buffer[1024] = { 0 };
    clientAddrSize = sizeof(clientAddr);

    while (true) {  // Loop to keep receiving packets
        int bytesReceived = recvfrom(serverSocket, buffer, sizeof(buffer) - 1, 0,
            (sockaddr*)&clientAddr, &clientAddrSize);
        std::cout << clientIP << "\t" << clientAddrSize << "\n";
        if (bytesReceived == SOCKET_ERROR) {
            //std::cerr << "Receive failed: " << WSAGetLastError() << "\n";
            break;
        }
        else {
            buffer[bytesReceived] = '\0';  // Null-terminate the string
            ButtonFunctionality::getCurrentSceneObject()->ReceiveInfo(buffer);

            inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
            std::cout << "Received from " << clientIP << ":" << ntohs(clientAddr.sin_port) << ": " << buffer << "\n";
        }
    }
}

void Server::CleanUp()
{
    closesocket(serverSocket);
    WSACleanup();
}

void Server::SendToClient(const char* message)
{
    //std::cout << serverSocket << "\t";
    clientAddr.sin_port = htons(8080);
    int sendResult = sendto(serverSocket, message, strlen(message), 0,
        (sockaddr*)&clientAddr, clientAddrSize);

    if (sendResult == SOCKET_ERROR) {
        //std::cerr << "Sendto failed: " << WSAGetLastError() << "\n";
    }
    else {
        //std::cout << "Sent response to " << clientIP << "\n";
    }
}
sockaddr_in Server::clientAddr = {};
int Server::clientAddrSize = 0;
SOCKET Server::serverSocket = 0;
char clientIP[22] = {};