#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <winsock2.h>

#pragma comment (lib, "ws2_32.lib")

class network {
public:
    network(const char* ip, int port) {
        // Initialize Winsock
        WSADATA wsData;
        WORD ver = MAKEWORD(2, 2);

        int wsOk = WSAStartup(ver, &wsData);
        if (wsOk != 0) {
            std::cerr << "Can't Initialize winsock! Quitting" << std::endl;
            return;
        }

        // Create a socket
        m_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (m_socket == INVALID_SOCKET) {
            std::cerr << "Can't create socket! Quitting" << std::endl;
            WSACleanup();
            return;
        }

        // Fill in a hint structure
        sockaddr_in hint;
        hint.sin_family = AF_INET;
        hint.sin_port = htons(port);
        inet_pton(AF_INET, ip, &hint.sin_addr);

        // Connect to the server
        int connResult = connect(m_socket, (sockaddr*)&hint, sizeof(hint));
        if (connResult == SOCKET_ERROR) {
            std::cerr << "Can't connect to server! Quitting" << std::endl;
            WSACleanup();
            return;
        }

        // Set the socket to be non-blocking
        u_long iMode = 1;
        ioctlsocket(m_socket, FIONBIO, &iMode);
    }

    ~network() {
        closesocket(m_socket);
        WSACleanup();
    }

    bool isConnected() {
        return m_connected;
    }

    void readMessage() {
        char buf[4096];
        ZeroMemory(buf, 4096);

        int bytesReceived = recv(m_socket, buf, 4096, 0);
        if (bytesReceived > 0) {
            std::cout << "SERVER> " << std::string(buf, 0, bytesReceived) << std::endl;
        }
    }

    void sendMessage(std::string msg) {
        send(m_socket, msg.c_str(), msg.size() + 1, 0);
    }

    void acceptConnection() {
        fd_set readSet;
        FD_ZERO(&readSet);
        FD_SET(m_socket, &readSet);

        timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 1000;

        int selectResult = select(0, &readSet, nullptr, nullptr, &timeout);
        if (selectResult == SOCKET_ERROR) {
            std::cerr << "Can't accept connection! Quitting" << std::endl;
            WSACleanup();
            return;
        }

        if (selectResult == 0) {
            return;
        }

        m_connected = true;
    }

private:
    SOCKET m_socket;
    bool m_connected = false;
};