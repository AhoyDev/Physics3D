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
		Load(&config);
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

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
	//console->LogConsole("Destroying SDL window and quitting all SDL systems\n");

	if(window)
		SDL_DestroyWindow(window);

	SDL_Quit();
	return true;
}

void ModuleWindow::Save(JSONNode* config)
{

}

void ModuleWindow::Load(JSONNode* config)
{
	title = config->PullString("window_title", "RubensEngine");
	width = config->PullInt("window_width", 1280);
	height = config->PullInt("window_height", 1024);
	flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

	if (config->PullBool("win_fullscreen", false))
		flags |= SDL_WINDOW_FULLSCREEN;

	if (config->PullBool("win_resizable", true))
		flags |= SDL_WINDOW_RESIZABLE;

	if (config->PullBool("win_borderless", false))
		flags |= SDL_WINDOW_BORDERLESS;

	if (config->PullBool("win_fullscreen_desktop", false))
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
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

bool ModuleWindow::CheckFlag(uint flag) const
{
	return flags & flag;
}


void ModuleWindow::SetBrightness(const float brightness)
{
	SDL_SetWindowBrightness(window, brightness);
}

void ModuleWindow::SetTitle(const char* new_title)
{
	if (new_title != nullptr)
	{
		SDL_SetWindowTitle(window, title);
		title = SDL_GetWindowTitle(window);
	}
}

void ModuleWindow::SetFullScreen(bool flag_value)
{
	if ((flags & SDL_WINDOW_FULLSCREEN) != flag_value)
	{
		Uint32 flag;
		
		if (flag_value)
		{
			flag = SDL_WINDOW_FULLSCREEN;
			flags |= SDL_WINDOW_FULLSCREEN;
		}
		else
		{
			flag = SDL_FALSE;
			flags -= SDL_WINDOW_FULLSCREEN;
		}

		SDL_SetWindowFullscreen(window, flag);
	}
}

void ModuleWindow::SetBorderless(bool flag_value)
{
	if ((flags & SDL_WINDOW_BORDERLESS) != flag_value)
	{
		SDL_bool flag;

		if (flag_value)
		{
			flag = SDL_TRUE;
			flags |= SDL_WINDOW_BORDERLESS;
		}
		else
		{
			flag = SDL_FALSE;
			flags -= SDL_WINDOW_BORDERLESS;
		}

		SDL_SetWindowBordered(window, flag);
	}
}

void ModuleWindow::SetFullDesktop(bool flag_value)
{
	if ((flags & SDL_WINDOW_FULLSCREEN) != flag_value)
	{
		Uint32 flag;

		if (flag_value)
		{
			flag = SDL_WINDOW_FULLSCREEN_DESKTOP;
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}
		else
		{
			flag = SDL_FALSE;
			flags -= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		SDL_SetWindowFullscreen(window, flag);
	}
}

void ModuleWindow::SetWindowSize(float _width, float _height)
{
	//SDL_SetWindowSize(window, _width, _height);
}

void ModuleWindow::SetWindowSizei(int new_width, int new_height)
{
	if (new_width >= 0 && new_height >= 0)
	{
		width = new_width;
		height = new_height;
		SDL_SetWindowSize(window, width, height);
	}
}



void ModuleWindow::SwapFullScreen()
{
	SetFullScreen(!(CheckFlag(SDL_WINDOW_FULLSCREEN)));
}

void ModuleWindow::SwapBorderless()
{
	SetBorderless(!(CheckFlag(SDL_WINDOW_BORDERLESS)));
}

void ModuleWindow::SwapFullDesktop()
{
	SetFullDesktop(!(CheckFlag(SDL_WINDOW_FULLSCREEN_DESKTOP)));
}
