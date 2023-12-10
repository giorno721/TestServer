#include <windows.h>
#include <iostream>
#include <string>

const std::string PIPE_NAME = "\\\\.\\pipe\\MyPipe";

void client() {
    HANDLE hPipe;

    while (true) {
        // Try to connect to the server until successful
        while (true) {
            hPipe = CreateFileA(
                PIPE_NAME.c_str(),
                GENERIC_READ | GENERIC_WRITE,
                0,
                NULL,
                OPEN_EXISTING,
                0,
                NULL
            );

            if (hPipe != INVALID_HANDLE_VALUE) {
                std::cout << "Connected to the server.\n";
                break;
            }
            else {
                std::cerr << "Error connecting to the server. Error code: " << GetLastError() << std::endl;
            }

            // Sleep before retrying
            Sleep(1000);
        }

        // Send a directory path to the server
        std::string directoryPath;
        std::cout << "Enter the directory path: ";
        std::getline(std::cin, directoryPath);

        DWORD bytesWritten;
        if (WriteFile(hPipe, directoryPath.c_str(), static_cast<DWORD>(directoryPath.size() + 1), &bytesWritten, NULL) != FALSE) {
            std::cout << "Directory path sent to the server: " << directoryPath << std::endl;
        }
        else {
            std::cerr << "Error writing to pipe. Error code: " << GetLastError() << std::endl;
        }

        // Receive the directory contents from the server
        char buffer[4096];
        DWORD bytesRead;

        if (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &bytesRead, NULL) != FALSE) {
            buffer[bytesRead] = '\0';
            std::cout << "Received directory contents from the server:\n" << buffer << std::endl;
        }
        else {
            std::cerr << "Error reading from pipe. Error code: " << GetLastError() << std::endl;
        }

        // Close the pipe handle
        CloseHandle(hPipe);
    }
}

int main() {
    client();
    return 0;
}
