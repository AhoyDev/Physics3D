#include "Globals.h"
#include "Application.h"
#include "GUI_Hardware.h"
#include "imgui/imgui.h"
#include "SDL\include\SDL.h"

GUI_Hardware::GUI_Hardware()
{
}

void GUI_Hardware::ShowHardwareMenu()
{
		ImGui::Begin("Config");

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


		}
		char title[25] = "Framerate";

		ImGui::End();


	
}
