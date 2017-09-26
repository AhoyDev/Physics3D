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

	if (ImGui::CollapsingHeader("Memory"))
		ShowMemory();

	if (ImGui::CollapsingHeader("OpenGL"))
		ShowGLOptions();


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
	if (ImGui::SliderInt("Max FPS", &max_fps, 0, 200, NULL))
		App->time->SetMaxFPS(max_fps);
	
	// FPS Plotter
	std::string title = "Framerate: ";
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
	
	title += std::to_string(lastFPS);
	ImGui::PlotHistogram("##Framerate", &fps[0], fps.size(), 0, title.c_str(), 0.f, fps_max_value, ImVec2(310, 100));

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
	ImGui::PlotHistogram("##Miliseconds", &ms[0], ms.size(), 0, title.c_str(), 0.0f, 30.0f, ImVec2(310, 100));
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
	ImGui::Text("GLSL: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));


	LOG("Vendor: %s", glGetString(GL_VENDOR));
	LOG("Renderer: %s", glGetString(GL_RENDERER));
	LOG("OpenGL version supported %s", glGetString(GL_VERSION));
	LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	//getGraphicsDeviceInfo(nullptr, nullptr, nullptr, &video_mem_budget, &video_mem_usage, &video_mem_available, &video_mem_reserved);
	//VRAM Budget
	//VRAM Usage
	//VRAM Available
	//VRAM Reserved
}

void GUI_Config::ShowMemory()
{
	sMStats stats = m_getMemoryStatistics();


	// FPS Plotter
	std::string title = "Memory Usage: ";
	unsigned int lastMemoryUsage = stats.totalReportedMemory;
	if (memory.size() > PLOTING_BARS)
	{
		for (int i = 1; i < memory.size(); i++)
		{
			memory[i - 1] = memory[i];
		}

		memory[memory.size() - 1] = lastMemoryUsage;
	}
	else
	{
		memory.push_back(lastMemoryUsage);
	}
	
	
	ImGui::Text("Total Reported Mem: %u", stats.totalReportedMemory);
	ImGui::Text("Total Actual Mem: %u", stats.totalActualMemory);
	ImGui::Text("Peak Reported Mem: %u", stats.peakReportedMemory);
	ImGui::Text("Peak Actual Mem: %u", stats.peakActualMemory);
	ImGui::Text("Accumulated Reported Mem: %u", stats.accumulatedReportedMemory);
	ImGui::Text("Accumulated Actual Mem: %u", stats.accumulatedActualMemory);
	ImGui::Text("Accumulated Alloc Unit Count: %u", stats.accumulatedAllocUnitCount);
	ImGui::Text("Total Alloc Unit Count: %u", stats.totalAllocUnitCount);
	ImGui::Text("Peak Alloc Unit Count: %u", stats.peakAllocUnitCount);



	ImGui::PlotHistogram("##Memory Usage", &memory[0], (int)memory.size(), 0, title.c_str(), 0.f, (float)stats.totalActualMemory, ImVec2(310, 100));

	//ImGui::PlotHistogram("##Framerate", &fps[0], (int)fps.size(), 0, title.c_str(), 0.f, fps_max_value, ImVec2(310, 100));
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

}
