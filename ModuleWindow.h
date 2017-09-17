#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"

class SDL_Window;
class SDL_Surface;

class ModuleWindow : public Module
{
public:

	ModuleWindow(bool start_enabled = true);
	~ModuleWindow();

	bool Init();
	bool CleanUp();

	void SetTitle(const char* title);

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;
};

#endif // __ModuleWindow_H__