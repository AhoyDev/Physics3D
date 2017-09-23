#ifndef __MODULEFS_H__
#define __MODULEFS_H__

#include "Module.h"
#include "PhysFS/include/physfs.h"

class ModuleWindowsFS : public Module
{
public:
	ModuleWindowsFS(const char* name, bool start_enabled = true);
	~ModuleWindowsFS();

	bool LoadFileToBuffer(char** buffer, const char* file = nullptr) const;
	void Save(const char* file, const void* buffer, unsigned int size) const;
};


class ModuleFS : public Module
{
public:
	ModuleFS(const char* name, bool start_enabled = true);
	~ModuleFS();

	bool AddPath(const char* path, const char* mount_point = nullptr);
	PHYSFS_sint64 LoadFileToBuffer(char** buffer, const char* file = nullptr) const;
	PHYSFS_sint64 Save(const char* file, const void* buffer, unsigned int size) const;
};

#endif