<<<<<<< HEAD
=======
#pragma once
>>>>>>> e1d043edc89add952ac34de3ccc0a8291abfb980
#ifndef CACHE_H
#define CACHE_H

#include <iostream>
#include <string>

class Cache {
public:
<<<<<<< HEAD
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
=======
	Cache();

	std::string GetPath() const;
	std::string GetFileExtension() const;
	std::string GetDirectoryContents() const;

	void SetPath(const std::string& newPath);
	void SetFileExtension(const std::string& newFileExtension);
	void SetDirectoryContents(const std::string& newDirectoryContents);

	void ClearData();

private:
	std::string path;
	std::string fileExtension;
	std::string directoryContents;
>>>>>>> e1d043edc89add952ac34de3ccc0a8291abfb980
};

#endif // CACHE_H
