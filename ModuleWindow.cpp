#include "ModuleWindow.h"

#include "Globals.h"
#include "SDL/include/SDL.h"

#include "GUI_Console.h"

ModuleWindow::ModuleWindow(const char* name, bool start_enabled) : Module(name, start_enabled),
title(nullptr), window(nullptr), screen_surface(nullptr)
{}

// Destructor
ModuleWindow::~ModuleWindow()
{}

// Called before render is available
bool ModuleWindow::Init(JSONNode config)
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		//Create window
		width = config.PullInt("window_width");
		height = config.PullInt("window_height");
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		if (config.PullBool("win_fullscreen"))
			flags |= SDL_WINDOW_FULLSCREEN;

		if (config.PullBool("win_resizable"))
			flags |= SDL_WINDOW_RESIZABLE;

		if (config.PullBool("win_borderless"))
			flags |= SDL_WINDOW_BORDERLESS;

		if (config.PullBool("win_fullscreen_desktop"))
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(!window)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	console->LogConsole("Destroying SDL window and quitting all SDL systems\n");

	if(window)
		SDL_DestroyWindow(window);

	SDL_Quit();
	return true;
}



int ModuleWindow::GetWidth() const
{
	return width;
}

int ModuleWindow::GetHeight() const
{
	return height;
}

float ModuleWindow::GetBrightness()const
{
	return SDL_GetWindowBrightness(window);
}

uint ModuleWindow::GetWindowFlags() const
{
	return SDL_GetWindowFlags(window);
}

bool ModuleWindow::CheckFlag(uint flag) const
{
	return SDL_GetWindowFlags(window) & flag;
}


void ModuleWindow::SetBrightness(const float brightness)
{
	SDL_SetWindowBrightness(window, brightness);
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

void ModuleWindow::SetFullScreen(bool flag_value)
{
	Uint32 flag = flag_value ? SDL_WINDOW_FULLSCREEN : SDL_FALSE;
	SDL_SetWindowFullscreen(window, flag);
}

void ModuleWindow::SetBorderless(bool flag_value)
{
	SDL_bool flag = flag_value ? SDL_TRUE : SDL_FALSE;
	SDL_SetWindowBordered(window, flag);
}

void ModuleWindow::SetFullDesktop(bool flag_value)
{
	Uint32 flag = flag_value ? SDL_WINDOW_FULLSCREEN_DESKTOP : SDL_FALSE;
	SDL_SetWindowFullscreen(window, flag);
}



void ModuleWindow::SwapFullScreen()
{
	bool flag_value = SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN;
	SetFullScreen(!flag_value);
}

void ModuleWindow::SwapBorderless()
{
	bool flag_value = SDL_GetWindowFlags(window) & SDL_WINDOW_BORDERLESS;
	SetBorderless(!flag_value);
}

void ModuleWindow::SwapFullDesktop()
{
	bool flag_value = SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN_DESKTOP;
	SetFullDesktop(!flag_value);
}
