#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

const int PORT = 12345;  // Use the same port as the server
const char* SERVER_IP = "127.0.0.1";  // Replace with the server's IP address

void client() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed.\n";
        return;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Error creating client socket. Error code: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    if (connect(clientSocket, reinterpret_cast<SOCKADDR*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed. Error code: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return;
    }

    std::cout << "Connected to the server.\n";

    while (true) {
        // Send a directory path to the server
        std::string directoryPath;
        std::cout << "Enter the directory path: ";
        std::getline(std::cin, directoryPath);

        if (send(clientSocket, directoryPath.c_str(), static_cast<int>(directoryPath.size()) + 1, 0) != SOCKET_ERROR) {
            std::cout << "Directory path sent to the server: " << directoryPath << std::endl;
        }
        else {
            std::cerr << "Error sending to server. Error code: " << WSAGetLastError() << std::endl;
            break;
        }

        // Receive the directory contents from the server
        char buffer[4096];
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0';
            std::cout << "Received directory contents from the server:\n" << buffer << std::endl;
        }
        else if (bytesRead == 0) {
            std::cerr << "Server disconnected.\n";
            break;
        }
        else {
            std::cerr << "Error receiving from server. Error code: " << WSAGetLastError() << std::endl;
            break;
        }
    }

    // Cleanup
    closesocket(clientSocket);
    WSACleanup();
}

//int main() {
//    client();
//    //lkjh
//    return 0;
//}
