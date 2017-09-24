#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"

class SDL_Window;
class SDL_Surface;

class ModuleWindow : public Module
{
public:

	ModuleWindow(const char* name, bool start_enabled = true);
	~ModuleWindow();

	bool Init(JSONNode config);
	bool CleanUp();

	int GetWidth() const;
	int GetHeight() const;
	float GetBrightness()const;
	uint GetWindowFlags() const;
	bool CheckFlag(uint flag) const;

	void SetBrightness(const float brightness);
	void SetTitle(const char* title);
	void SetFullScreen(const bool flag_value);
	void SetBorderless(const bool flag_value);
	void SetFullDesktop(const bool flag_value);

	void SwapFullScreen();
	void SwapBorderless();
	void SwapFullDesktop();

public:

	SDL_Window* window;
	SDL_Surface* screen_surface;

private:

	char*	title;
	int		width;
	int		height;

};

#endif // __ModuleWindow_H__