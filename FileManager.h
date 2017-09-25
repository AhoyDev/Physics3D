#ifndef __FILEMANAGER_H__
#define __FILEMANAGER_H__

#include "PhysFS/include/physfs.h"

class FileManager
{
public:
	FileManager();
	~FileManager();

	virtual bool LoadFileToBuffer(char** buffer, const char* file = nullptr) const;
	virtual bool Save(const char* file, const char* buffer, unsigned int size = 0) const;

	bool AddPath(const char* path, const char* mount_point = nullptr);
};

class WindowsFileManager : public FileManager
{
public:
	WindowsFileManager();
	~WindowsFileManager();

	bool LoadFileToBuffer(char** buffer, const char* file = nullptr) const;
	bool Save(const char* file, const char* buffer, unsigned int size = 0) const;
};

#endif