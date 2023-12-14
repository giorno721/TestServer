#ifndef CACHE_H
#define CACHE_H

#include <iostream>
#include <string>

class Cache {
public:
  Cache();

  std::string GetPath() const;
  std::string GetFileExtension() const;
  std::string GetDirectoryContents() const;

  void SetPath(const std::string &newPath);
  void SetFileExtension(const std::string &newFileExtension);
  void SetDirectoryContents(const std::string &newDirectoryContents);

  void ClearData();

private:
  std::string path;
  std::string fileExtension;
  std::string directoryContents;
};

#endif // CACHE_H
