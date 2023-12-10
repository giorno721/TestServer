#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <iostream>
#include <filesystem>
#include <string>
#include <iomanip>
#include <ctime>

const std::string PIPE_NAME = "\\\\.\\pipe\\MyPipe";

namespace fs = std::filesystem;

void server() {
    HANDLE hPipe = CreateNamedPipeA(
        PIPE_NAME.c_str(),
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
        1,
        0,
        0,
        0,
        NULL
    );

    if (hPipe == INVALID_HANDLE_VALUE) {
        std::cerr << "Error creating named pipe. Error code: " << GetLastError() << std::endl;
        return;
    }

    while (true) {
        std::cout << "Waiting for client to connect...\n";

        if (ConnectNamedPipe(hPipe, NULL) != FALSE) {
            std::cout << "Client connected.\n";

            char buffer[1024];
            DWORD bytesRead;

            // Read directory path from the client
            if (ReadFile(hPipe, buffer, sizeof(buffer), &bytesRead, NULL) != FALSE) {
                buffer[bytesRead] = '\0';
                std::string clientDirectory(buffer);
                std::cout << "Received directory path from client: " << clientDirectory << std::endl;

                // Read the directory contents
                std::string directoryContents;

                // Read the directory contents
                for (const auto& entry : std::filesystem::directory_iterator(clientDirectory)) {
                    auto time_point = std::chrono::time_point_cast<std::chrono::system_clock::duration>(entry.last_write_time() - fs::file_time_type::clock::now() + std::chrono::system_clock::now());
                    std::time_t time_t_value = std::chrono::system_clock::to_time_t(time_point);

                    std::tm localTime;
                    localtime_s(&localTime, &time_t_value);

                    std::ostringstream oss;
                    oss << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");

                    directoryContents += entry.path().filename().string() + " | "
                        + std::to_string(fs::file_size(entry)) + " bytes | "
                        + oss.str()
                        + "\n";
                }

                // Send directory contents to the client
                DWORD bytesWritten;
                if (WriteFile(hPipe, directoryContents.c_str(), static_cast<DWORD>(directoryContents.size()), &bytesWritten, NULL) != FALSE) {
                    std::cout << "Directory contents sent to the client.\n";
                }
                else {
                    std::cerr << "Error writing to pipe. Error code: " << GetLastError() << std::endl;
                }
            }
            else {
                std::cerr << "Error reading directory path from the client. Error code: " << GetLastError() << std::endl;
            }

            // Disconnect the pipe
            DisconnectNamedPipe(hPipe);
        }
    }

    // Close the pipe handle
    CloseHandle(hPipe);
}

int main() {
    server();
    return 0;
}
