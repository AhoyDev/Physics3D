#include "GUI_Config.h"
#include "imgui\imgui.h"
#include "SDL\include\SDL.h"
#include "Application.h"
#include "TimeManager.h"
#include "Globals.h"

#define MAX_BARS 25


GUI_Config::GUI_Config()
{}

GUI_Config::~GUI_Config()
{
	fps_plot_values.clear();
}

void GUI_Config::ShowConfigMenu()
{

	ImGui::SetNextWindowSize(ImVec2(500, 440));
	
	ImGui::Begin("Config");
	ImGui::Text("Options");
	if (ImGui::CollapsingHeader("Application"))
	{
		/*static char app_name[120];
		strcpy_s(app_name, 120, App->GetAppName());
		if (ImGui::InputText("App Name", app_name, 120, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
			App->SetAppName(app_name);*/
	}
	if (ImGui::CollapsingHeader("System"))
	{
		SDL_version ver;
		SDL_GetVersion(&ver);

		ImGui::Text("SDL version: %d.%d", ver.major, ver.minor);
		ImGui::Separator();
		ImGui::Text("CPUs: ");

		ImGui::SameLine();
		int i = SDL_GetCPUCount();
		ImGui::Text("%d cores", i);


		ImGui::Text("System RAM: ");
		i = SDL_GetSystemRAM();
		ImGui::SameLine();
		ImGui::Text("%d mb/s", i);

		ImGui::Text("Caps: ");
		bool isTrue = SDL_HasSSE();
		ImGui::SameLine();

		if (isTrue)
			ImGui::Text("SSE,");

		isTrue = SDL_HasSSE2();

		ImGui::SameLine();

		if (isTrue)
			ImGui::Text("SSE2,");

		isTrue = SDL_HasSSE3();

		ImGui::SameLine();

		if (isTrue)
			ImGui::Text("SSE3,");
		isTrue = SDL_HasSSE41();

		ImGui::SameLine();

		if (isTrue)
			ImGui::Text("SSE41,");
		isTrue = SDL_HasSSE42();

		ImGui::SameLine();

		if (isTrue)
			ImGui::Text("SSE42,");

		isTrue = SDL_HasAVX();



		if (isTrue)
			ImGui::Text("AVX");



		ImGui::Separator();

		ImGui::Text("GPU:");

		if (ImGui::CollapsingHeader("App"))
		{
			// FPS Plotter
			if (fps_plot_values.size() > 100)
			{
				for (int i = 1; i < fps_plot_values.size(); i++)
					fps_plot_values[i - 1] = fps_plot_values[i];

				fps_plot_values[fps_plot_values.size() - 1] = App->time->GetLastFPS();
			}
			else
			{
				fps_plot_values.push_back(App->time->GetLastFPS());
			}

			ImGui::PlotHistogram("Framerate", &fps_plot_values[0], fps_plot_values.size(), 0, NULL, 0.0f, 90.0f, ImVec2(310, 100));
		}


	}
	
	if (ImGui::CollapsingHeader("Window")){
		static char buf[19] = ""; ImGui::InputText("default", buf, 19);
		
		if (ImGui::RadioButton("FullScreen", false)) {}
		ImGui::SameLine();
		if (ImGui::RadioButton("resizable", false)) {}
		
		if (ImGui::RadioButton("borderless", false)) {}
		ImGui::SameLine();
		if (ImGui::RadioButton("fullscreenDesktop", false)) {}
	
	}
		
		
	if (ImGui::CollapsingHeader("Hardware")) {


	}



	ImGui::End();
}
