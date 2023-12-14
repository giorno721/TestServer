#include <iostream>
#include <string>

using namespace std;

class Cache
{
public:
  Cache() {}

  string GetPath() const
  {
	return path;
  }
  void SetPath(const string &newPath)
  {
	path = newPath;
  }

  string GetFileExtension() const
  {
	return fileExtension;
  }
  void SetFileExtension(const string &newFileExtension)
  {
	fileExtension = newFileExtension;
  }

  string GetDirectoryContents() const
  {
	return directoryContents;
  }
  void SetDirectoryContents(const string &newDirectoryContents)
  {
	directoryContents = newDirectoryContents;
  }

private:
  string path;
  string fileExtension;
  string directoryContents;
};