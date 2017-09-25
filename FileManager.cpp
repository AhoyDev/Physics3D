#include "FileManager.h"

#include "Globals.h"

#include "SDL/include/SDL_filesystem.h"
#include "SDL/include/SDL_stdinc.h"
#include "SDL\include\SDL_assert.h"

#pragma comment( lib, "PhysFS/libx86/physfs.lib" )

#include <fstream>
#include <sstream>

FileManager::FileManager()
{
	char* path = SDL_GetBasePath();
	SDL_assert(PHYSFS_init(path) != 0);
	SDL_free(path);

	// Add own directory to find Configuration.json later
	SDL_assert(AddPath("/"));
}

FileManager::~FileManager()
{
	PHYSFS_deinit();
}

bool FileManager::AddPath(const char* path, const char* mount_point)
{
	bool ret = true;

	if (PHYSFS_mount(path, mount_point, 1) == 0)
	{
		ret = false;
		LOG("FS Error - could't ADD PATH (%s): %s\n", path, PHYSFS_getLastError());
	}

	return ret;
}

bool FileManager::LoadFileToBuffer(char** buffer, const char* file) const
{
	bool ret = false;

	if (file == nullptr || !PHYSFS_exists(file))
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

				if (buffer) delete[] buffer;

				buffer = nullptr;
			}
			else
			{
				ret = true;
			}
		}

		if (PHYSFS_close(fs_file) <= 0)
		{
			LOG("FS Error - couldn't CLOSE %s: %s\n", file, PHYSFS_getLastError());
		}
	}

	return ret;
}

bool FileManager::Save(const char* file, const void* buffer, unsigned int size) const
{
	bool ret = false;

	PHYSFS_file* fs_file = PHYSFS_openWrite(file);

	if (fs_file == NULL)
	{
		LOG("FS Error - could't OPEN for saving %s: %s\n", file, PHYSFS_getLastError());
	}
	else
	{
		PHYSFS_sint64 written = PHYSFS_write(fs_file, (const void*)buffer, 1, size);

		if (written != size)
		{
			LOG("File System error while writing to file %s: %s\n", file, PHYSFS_getLastError());
		}
		else
		{
			ret = true;
		}

		if (PHYSFS_close(fs_file) == 0)
		{
			LOG("FS Error - couldn't CLOSE %s: %s\n", file, PHYSFS_getLastError());
		}
	}

	return ret;
}

WindowsFileManager::WindowsFileManager()
{}

WindowsFileManager::~WindowsFileManager()
{}

bool WindowsFileManager::LoadFileToBuffer(char** buffer, const char* file) const
{
	bool ret = false;

	std::ifstream ifs;
	ifs.open(file, std::ifstream::in);

	if (!ifs.is_open())
	{
		LOG("WindowsFS Error - could't OPEN %s\n", file);
	}
	else
	{
		std::stringstream iss;
		iss << ifs.rdbuf();

		iss.seekg(0, std::ios::end); // set stringstream position to file end
		uint size = iss.tellg(); // get size by getting offset position

		if (size == 0)
		{
			LOG("WindowsFS Error - could't DETERMINE SIZE for %s\n", file);
		}
		else
		{
			*buffer = new char[size];
			iss.str()._Copy_s(*buffer, size, size);
			ret = true;
		}

		ifs.close();
	}

	return ret;
}

bool WindowsFileManager::Save(const char* file, const void* buffer, unsigned int size) const
{
	return false;
}





