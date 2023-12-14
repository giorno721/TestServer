#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <filesystem>
#include <string>
#include <iomanip>
#include <ctime>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

namespace fs = std::filesystem;

const int PORT = 12345;  // Choose a port

std::vector<std::string> DiveInformation(char buffer[])
{
  std::string userInformation(buffer);
  std::istringstream iss(userInformation);
  std::vector<std::string> result;

  std::string token;
  while (std::getline(iss, token, '|')) {
	// Позбавтеся від зайвих пробілів на початку і в кінці кожної частини
	size_t start = token.find_first_not_of(" ");
	size_t end = token.find_last_not_of(" ");
	if (start != std::string::npos && end != std::string::npos) {
	  result.push_back(token.substr(start, end - start + 1));
	}
  }
  return result;
}

void server() {
  WSADATA wsaData;
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
	std::cerr << "WSAStartup failed.\n";
	return;
  }

  SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocket == INVALID_SOCKET) {
	std::cerr << "Error creating server socket. Error code: " << WSAGetLastError() << std::endl;
	WSACleanup();
	return;
  }

  sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = INADDR_ANY;
  serverAddr.sin_port = htons(PORT);

  if (bind(serverSocket, reinterpret_cast<SOCKADDR *>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
	std::cerr << "Bind failed. Error code: " << WSAGetLastError() << std::endl;
	closesocket(serverSocket);
	WSACleanup();
	return;
  }

  if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
	std::cerr << "Listen failed. Error code: " << WSAGetLastError() << std::endl;
	closesocket(serverSocket);
	WSACleanup();
	return;
  }

  std::cout << "Server is listening for incoming connections...\n";

  while (true) {
	SOCKET clientSocket = accept(serverSocket, NULL, NULL);
	if (clientSocket == INVALID_SOCKET) {
	  std::cerr << "Accept failed. Error code: " << WSAGetLastError() << std::endl;
	  closesocket(serverSocket);
	  WSACleanup();
	  return;
	}

	std::cout << "Client connected.\n";

	char buffer[1024];
	int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
	if (bytesRead > 0) {
	  buffer[bytesRead] = '\0';
	  auto userInformation = DiveInformation(buffer);

	  std::cout << "Received directory path from client: " << userInformation[0] << std::endl;
	  std::cout << "Received file extension from client: " << userInformation[1] << std::endl;

	  std::string directoryContents;
	  try {
		for (const auto &entry : std::filesystem::directory_iterator(userInformation[0]))
		{
		  if (entry.path().extension() == userInformation[1])
		  {
			auto time_point = std::chrono::time_point_cast<std::chrono::system_clock::duration>
			  (entry.last_write_time() - fs::file_time_type::clock::now() + std::chrono::system_clock::now());
			std::time_t time_t_value = std::chrono::system_clock::to_time_t(time_point);

			std::tm localTime;
			localtime_s(&localTime, &time_t_value);

			std::ostringstream oss;
			oss << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");

			directoryContents += entry.path().filename().string() + "\t|\t"
			  + std::to_string(fs::file_size(entry)) + " bytes\t|\t"
			  + oss.str()
			  + "\n";
		  }
		}
		if (!directoryContents.empty())
		{
		  send(clientSocket, directoryContents.c_str(), directoryContents.size(), 0);
		  std::cout << "Directory contents sent to the client.\n";
		}
		else
		{
		  std::cout << "File with " << userInformation[1] << " is not found." << std::endl;
		}
	  }
	  catch (const std::filesystem::filesystem_error &e) {
		std::cerr << "Error accessing directory: " << e.what() << std::endl;
		// Send an error message to the client if needed
	  }
	  catch (...)
	  {
		std::cout << "Error: " << GetLastError() << std::endl;
	  }
	}

	closesocket(clientSocket);
  }

  closesocket(serverSocket);
  WSACleanup();
}

int main() {
  server();
  return 0;
}
