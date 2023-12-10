#include <iostream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <chrono>
#include <ctime>

namespace fs = std::filesystem;

void printFileInfo(const fs::directory_entry& entry) {
    std::cout << "File Name: " << entry.path().filename().string() << std::endl;
    std::cout << "Size: " << fs::file_size(entry) << " bytes" << std::endl;

    auto lastWriteTime = fs::last_write_time(entry);
    //std::time_t ctime = decltype(lastWriteTime)::clock::to_time_t(lastWriteTime);
    //std::cout << "Last Modified: " << std::ctime(&ctime) << std::endl;

    //std::cout << "---------------------------\n";
}

void listFilesInDirectory(const std::string& path) {
    std::vector<fs::directory_entry> files;

    for (const auto& entry : fs::directory_iterator(path)) {
        if (entry.is_regular_file()) {
            files.push_back(entry);
        }
    }

    std::cout << "Total Files: " << files.size() << std::endl;

    if (!files.empty()) {
        std::cout << "\nFile Details:\n";
        for (const auto& file : files) {
            printFileInfo(file);
        }
    }
    else {
        std::cout << "No files found in the specified directory.\n";
    }
}

int main() {
    std::string directoryPath;

    std::cout << "Enter the directory path: ";
    std::cin >> directoryPath;

    listFilesInDirectory(directoryPath);

    return 0;
}