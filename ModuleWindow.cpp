#include "ModuleWindow.h"

#include "Application.h"
#include "SDL/include/SDL.h"
#include "JSONNode.h"
#include "GUI_Console.h"

#define BORDER_HEIGHT 20

ModuleWindow::ModuleWindow(const char* name, bool start_enabled) : Module(name, start_enabled),
window(nullptr), screen_surface(nullptr), flags(0)
{}

// Destructor
ModuleWindow::~ModuleWindow()
{}

// Called before render is available
bool ModuleWindow::Init()
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
		window = SDL_CreateWindow(
			App->config_values.title, // Title
			SDL_WINDOWPOS_UNDEFINED, // x position
			SDL_WINDOWPOS_UNDEFINED, // y position
			App->config_values.width, // width
			App->config_values.height, // height
			flags); // flags

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

void ModuleWindow::Save(JSONNode* config) const
{
	config->PushString("window_title", App->config_values.title);
	config->PushInt("window_width", App->config_values.width);
	config->PushInt("window_height", App->config_values.height);
	config->PushBool("win_fullscreen", App->config_values.fullScreen);
	config->PushBool("win_resizable", App->config_values.resizable);
	config->PushBool("win_borderless", App->config_values.borderless);
	config->PushBool("win_fullscreen_desktop", App->config_values.fullscreenDesktop);
}

void ModuleWindow::Load(JSONNode* config)
{
	flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
	App->config_values.title = config->PullString("window_title", "RubensEngine");
	App->config_values.width = config->PullInt("window_width", 1280);
	App->config_values.height = config->PullInt("window_height", 1024);

	if (App->config_values.fullScreen = config->PullBool("win_fullscreen", false))
		flags |= SDL_WINDOW_FULLSCREEN;

	if (App->config_values.resizable = config->PullBool("win_resizable", true))
		flags |= SDL_WINDOW_RESIZABLE;

	if (App->config_values.borderless = config->PullBool("win_borderless", false))
		flags |= SDL_WINDOW_BORDERLESS;

	if (App->config_values.fullscreenDesktop = config->PullBool("win_fullscreen_desktop", false))
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
}


int ModuleWindow::GetWidth() const
{
	return (flags & SDL_WINDOW_FULLSCREEN) ? GetMaxWidth() : App->config_values.width;
}

int ModuleWindow::GetHeight() const
{
	return (flags & SDL_WINDOW_FULLSCREEN) ? GetMaxHeight() : App->config_values.height;
}

int ModuleWindow::GetMaxWidth() const
{
	int ret = -1;

	SDL_DisplayMode mode;
	for (int i = 0; i < SDL_GetNumVideoDisplays(); ++i)
	{
		if (SDL_GetCurrentDisplayMode(i, &mode) == 0)
			ret = mode.w;
	}

	return ret;
}

int ModuleWindow::GetMaxHeight() const
{
	int ret = -1;

	SDL_DisplayMode mode;
	for (int i = 0; i < SDL_GetNumVideoDisplays(); ++i)
	{
		if (SDL_GetCurrentDisplayMode(i, &mode) == 0)
			ret = mode.h;
	}

	return ret;
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
		App->config_values.title = new_title;
		SDL_SetWindowTitle(window, App->config_values.title);
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

		SDL_SetWindowSize(window, GetMaxWidth(), GetMaxHeight());
		SDL_SetWindowFullscreen(window, flag);

	}
}

void ModuleWindow::SetResizeable(const bool flag_value)
{
	if ((flags & SDL_WINDOW_RESIZABLE) != flag_value)
	{
		if (flag_value)
			flags |= SDL_WINDOW_RESIZABLE;
		else
			flags -= SDL_WINDOW_RESIZABLE;
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
			int x, y;
			SDL_GetWindowPosition(window, &x, &y);
			SDL_SetWindowPosition(window, x, y + BORDER_HEIGHT);
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

void ModuleWindow::SetWindowSize(int new_width, int new_height)
{
	App->config_values.width = new_width;
	App->config_values.height = new_height;

	if (flags & SDL_WINDOW_RESIZABLE)
		SDL_SetWindowSize(window, new_width, new_height);
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
