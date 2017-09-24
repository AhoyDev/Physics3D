#include "GUI_Config.h"
#include "imgui\imgui.h"
#include "SDL\include\SDL.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "TimeManager.h"
#include "Globals.h"
#include <gl/GL.h>
#include <string>

#define PLOTING_BARS 100


GUI_Config::GUI_Config()
{

}

GUI_Config::~GUI_Config()
{
	fps.clear();
	ms.clear();
}

void GUI_Config::ShowConfigMenu()
{
	ImGui::SetNextWindowSize(ImVec2(500, 440));
	
	ImGui::Begin("Config");
	
	if (ImGui::CollapsingHeader("Application"))
		ShowApp();
	
	if (ImGui::CollapsingHeader("Window"))
		ShowWindow();
		
	if (ImGui::CollapsingHeader("Hardware"))
		ShowHardware();

	ImGui::End();
}

void GUI_Config::ShowApp()
{
	/*static char app_name[120];
	strcpy_s(app_name, 120, App->GetAppName());
	if (ImGui::InputText("App Name", app_name, 120, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
	App->SetAppName(app_name);*/

	// FPS Cap
	int max_fps = App->time->GetMaxFPS();
	if (ImGui::SliderInt("Max FPS", &max_fps, -1, 200, NULL))
		App->time->SetMaxFPS(max_fps);
	
	// FPS Plotter
	std::string title = "Framerate: ";
	float lastFPS = App->time->GetLastFPS();
	if (fps.size() > PLOTING_BARS)
	{
		for (int i = 1; i < fps.size(); i++)
			fps[i - 1] = fps[i];

		fps[fps.size() - 1] = lastFPS;
	}
	else
	{
		fps.push_back(lastFPS);
	}
	
	title += std::to_string(lastFPS);
	ImGui::PlotHistogram("##Framerate", &fps[0], fps.size(), 0, title.c_str(), 0.0f, 90.0f, ImVec2(310, 100));

	// MS Plotter
	title = "Miliseconds: ";
	float miliseconds = App->time->GetLastFrameMs();
	if (ms.size() > PLOTING_BARS)
	{
		for (int i = 1; i < ms.size(); i++)
			ms[i - 1] = ms[i];

		ms[ms.size() - 1] = miliseconds;
	}
	else
	{
		ms.push_back(miliseconds);
	}

	title += std::to_string(miliseconds);
	ImGui::PlotHistogram("##Miliseconds", &fps[0], fps.size(), 0, title.c_str(), 0.0f, 90.0f, ImVec2(310, 100));
}

void GUI_Config::ShowWindow()
{
	//static char buf[19] = ""; ImGui::InputText("default", buf, 19);

	//Brightness
	float brightness = App->window->GetBrightness();
	if (ImGui::SliderFloat("Brightness", &brightness, 0.0f, 1.0f))
		App->window->SetBrightness(brightness);

	float width = App->window->GetWidth();
	float widthPixels = App->window->GetWidth();
	if (ImGui::SliderFloat("Width", &width, 0.0f, 1.0f))
		App->window->SetWindowSize(width*widthPixels, App->window->GetHeight());

	float height = App->window->GetHeight();
	float heightPixels = App->window->GetHeight();
	if (ImGui::SliderFloat("Height", &height, 0.0f, 1.0f))
		App->window->SetWindowSize(App->window->GetWidth(),height*heightPixels);





	// Window Flags
	if (ImGui::RadioButton("FullScreen", App->window->CheckFlag(SDL_WINDOW_FULLSCREEN)))
		App->window->SwapFullScreen();

	ImGui::SameLine();

	if (ImGui::RadioButton("Resizable", App->window->CheckFlag(SDL_WINDOW_RESIZABLE))) {}

	if (ImGui::RadioButton("Borderless", App->window->CheckFlag(SDL_WINDOW_BORDERLESS)))
		App->window->SwapBorderless();

	ImGui::SameLine();

	if (ImGui::RadioButton("FullscreenDesktop", App->window->CheckFlag(SDL_WINDOW_FULLSCREEN_DESKTOP)))
		App->window->SwapFullDesktop();
}

void GUI_Config::ShowHardware()
{
	// SDL Versions
	SDL_version ver;
	SDL_VERSION(&ver);
	ImGui::Text("Compiled SDL version: %d.%d.%d", ver.major, ver.minor, ver.patch);
	SDL_GetVersion(&ver);
	ImGui::Text("Linked SDL version: %d.%d.%d", ver.major, ver.minor, ver.patch);
	ImGui::Text("SDL Revision: %s", SDL_GetRevision());
	ImGui::Separator();

	// CPU
	ImGui::Text("CPUs: %d cores", SDL_GetCPUCount());

	// RAM
	int ram = SDL_GetSystemRAM();
	if (ram > KILOBYTE)
		ImGui::Text("System RAM: %f Gb/s", ram/KILOBYTE_F);
	else
		ImGui::Text("System RAM: %d Mb/s", ram);

	// Caps
	std::string caps = "Caps: ";
	if (SDL_Has3DNow()) caps += "3DNow, ";
	if (SDL_HasAVX()) caps += "AVX, ";
	if (SDL_HasAltiVec()) caps += "AltiVec, ";
	if (SDL_HasMMX()) caps += "MMX, ";
	if (SDL_HasRDTSC()) caps += "RDTSC, ";
	ImGui::Text(caps.c_str());

	caps = "";
	if (SDL_HasSSE()) caps += "SSE, ";
	if (SDL_HasSSE2()) caps += "SSE2, ";
	if (SDL_HasSSE3()) caps += "SSE3, ";
	if (SDL_HasSSE41()) caps += "SSE41, ";
	if (SDL_HasSSE42()) caps += "SSE42, ";
	ImGui::Text(caps.c_str());
	ImGui::Separator();

	// Render Drivers
	std::string renderDrivers = "Render Drivers: ";
	SDL_RendererInfo info;
	for (int i = 0; i < SDL_GetNumRenderDrivers() - 1 && SDL_GetRenderDriverInfo(i, &info) == 0; i++)
	{
		renderDrivers += info.name;
		renderDrivers += ", ";
	}
	SDL_GetRenderDriverInfo(SDL_GetNumRenderDrivers(), &info);
	renderDrivers += info.name;
	ImGui::Text(renderDrivers.c_str());
	ImGui::Separator();

	// GPU
	ImGui::Text("GPU: %s", glGetString(GL_RENDERER));
	ImGui::Text("Brand: %s", glGetString(GL_VENDOR));
	ImGui::Text("Version: %s", glGetString(GL_VERSION));

	//VRAM Budget
	//VRAM Usage
	//VRAM Available
	//VRAM Reserved
}
