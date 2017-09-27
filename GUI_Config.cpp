#include "GUI_Config.h"
#include "imgui\imgui.h"
#include "SDL\include\SDL.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "TimeManager.h"
#include "Globals.h"
#include "Glew\include\glew.h"
#include <gl/GL.h>
#include <string>
#include "mmgr\mmgr.h"
#include "gpudetect\DeviceId.h"

#define PLOTING_BARS 100

struct AppInfo
{
	std::string title = "Framerate: ";
	std::string renderDrivers = "Render Drivers: ";
	std::string caps = "Caps: ";
	std::string memory_usage = "Memory Usage: ";

}; AppInfo info_app;

bool ConfigValues::NeedSaving()
{
	return (config_max_fps != max_fps ||
		config_width != width ||
		config_height != height ||
		config_vsync != vsync ||
		config_fullScreen != fullScreen ||
		config_resizable != resizable ||
		config_borderless != borderless ||
		config_fullscreenDesktop != fullscreenDesktop);
}

bool ConfigValues::NeedRestart()
{
	return false;
}

GUI_Config::GUI_Config(const bool active) : GUI_Window(active)
{}

GUI_Config::~GUI_Config()
{
	fps.clear();
	ms.clear();
	mem.clear();
}

void GUI_Config::Draw()
{
	ImGui::SetNextWindowSize(ImVec2(500, 440));
	
	ImGui::Begin("Config");
	
	if (values.NeedSaving()) // if changes only have to be saved
	{
		if (ImGui::Button("Save Changes"))
			App->RequestSave();
	}
	else if (values.NeedRestart()) // if changes need restart to change
	{
		if (ImGui::Button("Restart and Save Changes"))
			App->RequestRestart();
	}

	if (ImGui::CollapsingHeader("Application"))
		ShowApp();
	
	if (ImGui::CollapsingHeader("Window"))
		ShowWindow();
		
	if (ImGui::CollapsingHeader("Hardware"))
		ShowHardware();

	if (ImGui::CollapsingHeader("Memory"))
		ShowMemory();

	if (ImGui::CollapsingHeader("OpenGL"))
		ShowGLOptions();


	ImGui::End();
}

void GUI_Config::SetConfigValues()
{
	values.config_max_fps = values.max_fps = App->time->GetMaxFPS();
	values.config_width = values.width = App->window->GetWidth();
	values.config_height = values.height = App->window->GetHeight();
	values.config_vsync = values.vsync = (SDL_GL_GetSwapInterval() == 1);
	values.config_fullScreen = values.fullScreen = App->window->CheckFlag(SDL_WINDOW_FULLSCREEN);
	values.config_resizable = values.resizable = App->window->CheckFlag(SDL_WINDOW_RESIZABLE);
	values.config_borderless = values.borderless = App->window->CheckFlag(SDL_WINDOW_BORDERLESS);
	values.config_fullscreenDesktop = values.fullscreenDesktop = App->window->CheckFlag(SDL_WINDOW_FULLSCREEN_DESKTOP);
}

void GUI_Config::ShowApp()
{
	info_app.title = "FrameRate: ";

	// FPS Cap
	if (ImGui::SliderInt("Max FPS", &values.max_fps, 0, 200, NULL))
		App->time->SetMaxFPS(values.max_fps);
	
	// FPS Plotter
	
	float lastFPS = App->time->GetLastFPS();
	float fps_max_value = 0.0f;
	if (fps.size() > PLOTING_BARS)
	{
		for (int i = 1; i < fps.size(); i++)
		{
			fps[i - 1] = fps[i];
			if (fps[i] > fps_max_value)
				fps_max_value = fps[i];
		}
		fps_max_value += fps_max_value * 0.20f;

		fps[fps.size() - 1] = lastFPS;
	}
	else
	{
		fps.push_back(lastFPS);
	}
	
	info_app.title += std::to_string(lastFPS);
	ImGui::PlotHistogram("##Framerate", &fps[0], fps.size(), 0, info_app.title.c_str(), 0.f, fps_max_value, ImVec2(310, 100));

	// MS Plotter
	info_app.title = "Miliseconds: ";
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

	info_app.title += std::to_string(miliseconds);
	ImGui::PlotHistogram("##Miliseconds", &ms[0], ms.size(), 0, info_app.title.c_str(), 0.0f, 30.0f, ImVec2(310, 100));
}

void GUI_Config::ShowWindow()
{
	//Brightness
	float brightness = App->window->GetBrightness();
	if (ImGui::SliderFloat("Brightness", &brightness, 0.0f, 1.0f))
		App->window->SetBrightness(brightness);
	
	// Window Dimensions
	if (values.resizable)
	{
		values.width = App->window->GetWidth();
		if (ImGui::SliderInt("Width", &values.width, 0, App->window->GetMaxWidth()))
			App->window->SetWindowSize(values.width, App->window->GetHeight());

		values.height = App->window->GetHeight();
		if (ImGui::SliderInt("Height", &values.height, 0, App->window->GetMaxHeigth()))
			App->window->SetWindowSize(App->window->GetHeight(), values.height);
	}
	else
	{
		ImGui::Text("Width: %d", values.width);
		ImGui::Text("Height: %d", values.height);
	}

	// VSYNC
	if (ImGui::RadioButton("VSYNC", values.vsync))
	{
		values.vsync = !values.vsync;
		SDL_GL_SetSwapInterval(values.vsync ? 1 : 0);
	}

	// Window Flags
	if (ImGui::RadioButton("FullScreen", values.fullScreen))
	{
		values.fullScreen = !values.fullScreen;
		App->window->SwapFullScreen();
	}

	ImGui::SameLine();

	
	if (ImGui::RadioButton("Resizable", values.resizable))
		values.resizable = !values.resizable;

	if (ImGui::RadioButton("Borderless", values.borderless))
	{
		values.borderless = !values.borderless;
		App->window->SwapBorderless();
	}

	ImGui::SameLine();
	
	if (ImGui::RadioButton("FullscreenDesktop", values.fullscreenDesktop))
	{
		values.fullscreenDesktop = !values.fullscreenDesktop;
		App->window->SwapFullDesktop();
	}
}

void GUI_Config::ShowHardware()
{
	// CPU
	ImGui::Text("CPUs: %d cores", SDL_GetCPUCount());

	// RAM
	int ram = SDL_GetSystemRAM();
	if (ram > KILOBYTE)
		ImGui::Text("System RAM: %f Gb/s", ram/KILOBYTE_F);
	else
		ImGui::Text("System RAM: %d Mb/s", ram);

	// Caps
	
	if (SDL_Has3DNow()) info_app.caps += "3DNow, ";
	if (SDL_HasAVX()) info_app.caps += "AVX, ";
	if (SDL_HasAltiVec()) info_app.caps += "AltiVec, ";
	if (SDL_HasMMX()) info_app.caps += "MMX, ";
	if (SDL_HasRDTSC()) info_app.caps += "RDTSC, ";
	ImGui::Text(info_app.caps.c_str());

	info_app.caps = "";
	if (SDL_HasSSE()) info_app.caps += "SSE, ";
	if (SDL_HasSSE2()) info_app.caps += "SSE2, ";
	if (SDL_HasSSE3()) info_app.caps += "SSE3, ";
	if (SDL_HasSSE41()) info_app.caps += "SSE41, ";
	if (SDL_HasSSE42()) info_app.caps += "SSE42, ";
	ImGui::Text(info_app.caps.c_str());
	ImGui::Separator();

	// Render Drivers
	info_app.renderDrivers = "Render Drivers: ";
	SDL_RendererInfo info;
	for (int i = 0; i < SDL_GetNumRenderDrivers() - 1 && SDL_GetRenderDriverInfo(i, &info) == 0; i++)
	{
		info_app.renderDrivers += info.name;
		info_app.renderDrivers += ", ";
	}
	SDL_GetRenderDriverInfo(SDL_GetNumRenderDrivers(), &info);
	info_app.renderDrivers += info.name;
	ImGui::Text(info_app.renderDrivers.c_str());
	ImGui::Separator();

	// GPU
	ImGui::Text("GPU: %s", glGetString(GL_RENDERER));
	ImGui::Text("Brand: %s", glGetString(GL_VENDOR));
	ImGui::Text("Version: %s", glGetString(GL_VERSION));
	ImGui::Text("GLSL: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

	// VRAM
	unsigned __int64	video_memory_total;
	unsigned __int64	video_memory_use;
	unsigned __int64	video_mem_available;
	unsigned __int64	video_mem_reserved;
	getGraphicsDeviceInfo(nullptr, nullptr, nullptr, &video_memory_total, &video_memory_use, &video_mem_available, &video_mem_reserved);
	ImGui::Text("Total VRAM: %f", (float(video_memory_total)/ MEGABYTE_F));
	ImGui::Text("VRAM used: %f", (float(video_memory_use)/ MEGABYTE_F));
	ImGui::Text("VRAM available: %f", (float(video_mem_available)/ MEGABYTE_F));
	ImGui::Text("VRAM reserved: %f", (float(video_mem_reserved)/ MEGABYTE_F));

	
}

void GUI_Config::ShowMemory()
{
	sMStats stats = m_getMemoryStatistics();
	unsigned int lastMemoryUsage = stats.totalReportedMemory;
	if (mem.size() > PLOTING_BARS)
	{
		for (int i = 1; i < mem.size(); i++)
		{
			mem[i - 1] = mem[i];
		}

		mem[mem.size() - 1] = lastMemoryUsage;
	}
	else
	{
		mem.push_back(lastMemoryUsage);
	}

	ImGui::PlotHistogram("##Memory Usage", &mem[0], (int)mem.size(), 0, info_app.memory_usage.c_str(), 0.f, (float)stats.totalActualMemory, ImVec2(310, 100));
	
	ImGui::Text("Total Reported Mem: %u", stats.totalReportedMemory);
	ImGui::Text("Total Actual Mem: %u", stats.totalActualMemory);
	ImGui::Text("Peak Reported Mem: %u", stats.peakReportedMemory);
	ImGui::Text("Peak Actual Mem: %u", stats.peakActualMemory);
	ImGui::Text("Accumulated Reported Mem: %u", stats.accumulatedReportedMemory);
	ImGui::Text("Accumulated Actual Mem: %u", stats.accumulatedActualMemory);
	ImGui::Text("Accumulated Alloc Unit Count: %u", stats.accumulatedAllocUnitCount);
	ImGui::Text("Total Alloc Unit Count: %u", stats.totalAllocUnitCount);
	ImGui::Text("Peak Alloc Unit Count: %u", stats.peakAllocUnitCount);
}

void GUI_Config::ShowGLOptions()
{
	/*GL_DEPTH_TEST, GL_CULL_FACE, GL_LIGHTING GL_COLOR_MATERIAL, GL_TEXTURE_2D + two other
		Add options to draw in wireframe mode*/

	if (ImGui::RadioButton("GL Depth Test", App->renderer3D->getDepthTest()))
		App->renderer3D->setDepthTest();

	if (ImGui::RadioButton("GL Cull Face", App->renderer3D->getCullFace()))
		App->renderer3D->setCullFace();

	if (ImGui::RadioButton("GL Lightning", App->renderer3D->getGLLightning()))
		App->renderer3D->setGLLightning();

	if (ImGui::RadioButton("GL Color Material", App->renderer3D->getGLColorMaterial()))
		App->renderer3D->setGLColorMaterial();

	if (ImGui::RadioButton("GL Texture 2D", App->renderer3D->getGLTexture2D()))
		App->renderer3D->setGLTexture2D();

	if (ImGui::RadioButton("WireFrame mode", App->renderer3D->getWireFrame()))
		App->renderer3D->setWireFrame();

}
