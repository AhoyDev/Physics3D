#include "ModuleFS.h"

#include "SDL/include/SDL_filesystem.h"
#include "SDL/include/SDL_stdinc.h"
#include "SDL\include\SDL_assert.h"

#pragma comment( lib, "PhysFS/libx86/physfs.lib" )

ModuleFS::ModuleFS(bool start_enabled) : Module(start_enabled)
{
	char* path = SDL_GetBasePath();
	LOG(path);
	SDL_assert_paranoid(PHYSFS_init(path) != 0);
	SDL_free(path);

	//AddPath(".");
}

ModuleFS::~ModuleFS()
{
	PHYSFS_deinit();
}

bool ModuleFS::AddPath(const char* path, const char* mount_point)
{
	bool ret = true;

	if (PHYSFS_mount(path, mount_point, 1) == 0)
	{
		ret = false;
		LOG("FS Error - could't ADD PATH (%s): %s\n", path, PHYSFS_getLastError());
	}

	return ret;
}

PHYSFS_sint64 ModuleFS::LoadFileToBuffer(char** buffer, const char* file) const
{
	PHYSFS_sint64 ret = -1;

	if (file == nullptr || PHYSFS_exists(file) == NULL)
		return ret;

	PHYSFS_file* fs_file = PHYSFS_openRead(file);

	if (fs_file == NULL)
	{
		LOG("FS Error - could't OPEN %s: %s\n", file, PHYSFS_getLastError());
	}
	else
	{
		PHYSFS_sint64 size = PHYSFS_fileLength(fs_file);

		if (size < 0)
		{
			LOG("FS Error - could't DETERMINE SIZE for %s: %s\n", file, PHYSFS_getLastError());
		}
		else
		{
			*buffer = new char[(uint)size];
			PHYSFS_sint64 readed = PHYSFS_read(fs_file, *buffer, 1, (PHYSFS_sint32)size);

			if (readed != size)
			{
				LOG("FS Error - coun't READ %s: %s\n", file, PHYSFS_getLastError());

				if (buffer)
					delete[] buffer;
			}
			else
			{
				ret = readed;
			}
		}

		if (PHYSFS_close(fs_file) <= 0)
		{
			LOG("FS Error - couldn't CLOSE %s: %s\n", file, PHYSFS_getLastError());
		}
	}

	return ret;
}