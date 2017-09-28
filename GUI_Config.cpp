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

#define PLOTING_BARS 49

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
	
	if (App->config_values.NeedSaving()) // if changes only have to be saved
	{
		if (ImGui::Button("Save Changes"))
			App->RequestSave();
	}
	else if (App->config_values.NeedRestart()) // if changes need restart to change
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

void GUI_Config::ShowApp()
{
	// FPS Cap
	if (ImGui::SliderInt("Max FPS", &App->config_values.max_fps, 0, 200, NULL))
		App->time->SetMaxFPS(App->config_values.max_fps);
	
	char title[25];

	// FPS Plotter
	float max = AddPlotValue(fps, App->time->GetLastFPS());
	sprintf_s(title, 25, "Framerate %d", App->time->GetLastFPS());
	ImGui::PlotHistogram("##Framerate", &fps[0], fps.size(), 0, title, 0.f, 1.2f * App->config_values.max_fps, ImVec2(310, 100));

	// MS Plotter
	max = AddPlotValue(ms, App->time->GetLastFrameMs());
	sprintf_s(title, 25, "Miliseconds %0.1f", App->time->GetLastFrameMs());
	ImGui::PlotHistogram("##Miliseconds", &ms[0], ms.size(), 0, title, 0.0f, 30.0f, ImVec2(310, 100));
}

void GUI_Config::ShowWindow()
{
	//Brightness
	float brightness = App->window->GetBrightness();
	if (ImGui::SliderFloat("Brightness", &brightness, 0.0f, 1.0f))
		App->window->SetBrightness(brightness);
	
	// Window Dimensions
	if (App->config_values.resizable || !App->config_values.fullScreen || !App->config_values.fullscreenDesktop)
	{
		App->config_values.width = App->window->GetWidth();
		if (ImGui::SliderInt("Width", &App->config_values.width, 0, App->window->GetMaxWidth()))
			App->window->SetWindowSize(App->config_values.width, App->window->GetHeight());

		App->config_values.height = App->window->GetHeight();
		if (ImGui::SliderInt("Height", &App->config_values.height, 0, App->window->GetMaxHeight()))
			App->window->SetWindowSize(App->window->GetHeight(), App->config_values.height);
	}
	else
	{
		ImGui::Text("Width: %d", App->config_values.width);
		ImGui::Text("Height: %d", App->config_values.height);
	}

	// VSYNC
	if (ImGui::RadioButton("VSYNC", App->config_values.vsync))
	{
		App->config_values.vsync = !App->config_values.vsync;
		SDL_GL_SetSwapInterval(App->config_values.vsync ? 1 : 0);
	}

	// Window Flags
	if (ImGui::RadioButton("FullScreen", App->config_values.fullScreen))
	{
		App->config_values.fullScreen = !App->config_values.fullScreen;
		App->window->SwapFullScreen();
	}

	ImGui::SameLine();

	
	if (ImGui::RadioButton("Resizable", App->config_values.resizable))
		App->config_values.resizable = !App->config_values.resizable;

	if (ImGui::RadioButton("Borderless", !App->config_values.borderless))
	{
		App->config_values.borderless = !App->config_values.borderless;
		App->window->SwapBorderless();
	}

	ImGui::SameLine();
	
	if (ImGui::RadioButton("FullscreenDesktop", App->config_values.fullscreenDesktop))
	{
		App->config_values.fullscreenDesktop = !App->config_values.fullscreenDesktop;
		App->window->SwapFullDesktop();
	}
}

void GUI_Config::ShowHardware()
{
	// CPU
	ImGui::Text(cpus.c_str());

	// RAM
	ImGui::Text(ram.c_str());

	// Caps
	ImGui::Text(caps1.c_str());
	ImGui::Text(caps2.c_str());

	// Render Drivers
	ImGui::Text(drivers.c_str());
	ImGui::Separator();

	// GPU
	ImGui::Text(gpu_renderer.c_str());
	ImGui::Text(gpu_vendor.c_str());
	ImGui::Text(gpu_version.c_str());
	ImGui::Text(gpu_shading.c_str());

	// VRAM
	ImGui::Text(vram_total.c_str());
	ImGui::Text(vram_used.c_str());
	ImGui::Text(vram_available.c_str());
	ImGui::Text(vram_reserved.c_str());
}

void GUI_Config::ShowMemory()
{
	// Mem Plotter
	char title[25];
	sMStats stats = m_getMemoryStatistics();
	int max = AddPlotValue(mem, stats.totalReportedMemory);
	sprintf_s(title, 25, "Memory Usage %d", stats.totalReportedMemory);
	ImGui::PlotHistogram("##Memory Usage", &mem[0], mem.size(), 0, title, 0.f, 1.2f * max, ImVec2(310, 100));

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

void GUI_Config::UpdateHardware()
{
	// CPU
	cpus = "CPUs: ";
	cpus += std::to_string(SDL_GetCPUCount());
	cpus += " cores";

	// RAM
	char ram_s[25];
	float system_ram = SDL_GetSystemRAM();
	if (system_ram > KILOBYTE)
		sprintf_s(ram_s, 25, "System RAM %0.1f Gb/s", system_ram / KILOBYTE_F);
	else
		sprintf_s(ram_s, 25, "System RAM %0.1f Mb/s", system_ram);
	ram = ram_s;

	// Caps
	if (SDL_Has3DNow()) caps1 += " 3DNow";
	if (SDL_HasAVX()) caps1 += " AVX";
	if (SDL_HasAltiVec()) caps1 += " AltiVec";
	if (SDL_HasMMX()) caps1 += " MMX";
	if (SDL_HasRDTSC()) caps1 += " RDTSC";

	if (SDL_HasSSE()) caps2 += " SSE";
	if (SDL_HasSSE2()) caps2 += " SSE2";
	if (SDL_HasSSE3()) caps2 += " SSE3";
	if (SDL_HasSSE41()) caps2 += " SSE41";
	if (SDL_HasSSE42()) caps2 += " SSE42";

	// Render Drivers
	drivers = "Render Drivers: ";
	SDL_RendererInfo info;
	if (SDL_GetRenderDriverInfo(0, &info) != -1) drivers += info.name;
	for (int i = 1; i < SDL_GetNumRenderDrivers() && SDL_GetRenderDriverInfo(i, &info) == 0; i++)
	{
		drivers += ", ";
		drivers += info.name;
	}

	// GPU
	gpu_renderer = "GPU: ";
	gpu_renderer += (char*)glGetString(GL_RENDERER);
	gpu_vendor = "Brand: ";
	gpu_vendor += (char*)glGetString(GL_VENDOR);
	gpu_version = "Version:: ";
	gpu_version += (char*)glGetString(GL_VERSION);
	gpu_shading = "GLSL: ";
	gpu_shading += (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);

	// VRAM
	UpdateVRAM();
}

void GUI_Config::UpdateVRAM()
{
	unsigned __int64 total, used, available, reserved;
	getGraphicsDeviceInfo(nullptr, nullptr, nullptr, &total, &used, &available, &reserved);

	// Total
	vram_total = "Total VRAM: ";
	if (total > GIGABYTE)
	{
		vram_total += std::to_string(total / GIGABYTE_F);
		vram_total += " Gbs";
	}
	else if (total > GIGABYTE)
	{
		vram_total += std::to_string(total / MEGABYTE_F);
		vram_total += " Mbs";
	}
	else
	{
		vram_total += std::to_string(total);
		vram_total += " Kbs";
	}

	// Used
	vram_used = "VRAM used: ";
	if (used > GIGABYTE)
	{
		vram_used += std::to_string(used / GIGABYTE_F);
		vram_used += " Gbs";
	}
	else if (used > GIGABYTE)
	{
		vram_used += std::to_string(used / MEGABYTE_F);
		vram_used += " Mbs";
	}
	else
	{
		vram_used += std::to_string(used);
		vram_used += " Kbs";
	}

	// Available
	vram_available = "VRAM available: ";
	if (available > GIGABYTE)
	{
		vram_available += std::to_string(available / GIGABYTE_F);
		vram_available += " Gbs";
	}
	else if (available > GIGABYTE)
	{
		vram_available += std::to_string(available / MEGABYTE_F);
		vram_available += " Mbs";
	}
	else
	{
		vram_available += std::to_string(available);
		vram_available += " Kbs";
	}

	// Reserved
	vram_reserved = "VRAM reserved: ";
	if (reserved > GIGABYTE)
	{
		vram_reserved += std::to_string(reserved / GIGABYTE_F);
		vram_reserved += " Gbs";
	}
	else if (reserved > GIGABYTE)
	{
		vram_reserved += std::to_string(reserved / MEGABYTE_F);
		vram_reserved += " Mbs";
	}
	else
	{
		vram_reserved += std::to_string(reserved);
		vram_reserved += " Kbs";
	}
}

float GUI_Config::AddPlotValue(std::vector<float> &vec, float value)
{
	if (vec.empty())
	{
		vec.push_back(value);
		return value;
	}

	float max = vec[0];

	if (vec.size() > PLOTING_BARS)
	{
		for (int i = 1; i < vec.size(); i++)
		{
			vec[i - 1] = vec[i];

			if (vec[i] > max)
				max = vec[i];
		}

		vec.back() = value;
	}
	else
	{
		vec.push_back(value);

		for (int i = 0; i < vec.size(); i++)
		{
			if (vec[i] > max)
				max = vec[i];
		}
	}

	return max;
}