#ifndef __MODULEFS_H__
#define __MODULEFS_H__

#include "Module.h"
#include "PhysFS/include/physfs.h"

class ModuleFS : public Module
{
public:
	ModuleFS(bool start_enabled = true);
	~ModuleFS();

	bool AddPath(const char* path, const char* mount_point = nullptr);
	PHYSFS_sint64 LoadFileToBuffer(char** buffer, const char* file = nullptr) const;
};


#endif