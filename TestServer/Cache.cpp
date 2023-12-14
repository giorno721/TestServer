#include "Cache.h"


Cache::Cache() {
}

std::string Cache::GetPath() const {
  return path;
}

std::string Cache::GetFileExtension() const {
  return fileExtension;
}

std::string Cache::GetDirectoryContents() const {
  return directoryContents;
}

void Cache::SetPath(const std::string &newPath) {
  path = newPath;
}

void Cache::SetFileExtension(const std::string &newFileExtension) {
  fileExtension = newFileExtension;
}

void Cache::SetDirectoryContents(const std::string &newDirectoryContents) {
  directoryContents = newDirectoryContents;
}

void Cache::ClearData() {
  path = "";
  fileExtension = "";
  directoryContents = "";
  std::cout << "Cache data cleared!" << std::endl;
}