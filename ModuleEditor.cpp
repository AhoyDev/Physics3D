#include "ModuleEditor.h"

#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "SDL\include\SDL_keycode.h"
#include "GUI_Window.h"
#include "GUI_Tests.h"
#include "GUI_Config.h"
#include "GUI_Console.h"
#include "imgui\imgui.h"

struct Colors
{
	ImVec4 white;
	ImVec4 Black;
	ImVec4 yellow;
	ImVec4 skyBlue;
	ImVec4 blue;
	ImVec4 darkBlue;
	ImVec4 pink;
	ImVec4 Green;
	ImVec4 LighterGreen;
	ImVec4 DarkerGreen;
	ImVec4 AlphaGreen;
	ImVec4 red;

	Colors()
	{
		white = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		Black = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
		yellow = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
		skyBlue = ImVec4(0.10f, 0.10f, 1.00f, 1.0f);
		blue = ImVec4(0.3f, 0.0f, 0.0f, 1.0f);
		darkBlue = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
		pink = ImVec4(1.0f, 0.0f, 1.0f, 1.0f);
		Green = ImVec4(0, 0.88f, 0.73f, 1.00f);
		LighterGreen = ImVec4(0.5f, 1.0f, 0.5f, 1.0f);
		DarkerGreen = ImVec4(0.05f, 0.4f, 0.05f, 1.0f);
		AlphaGreen = ImVec4(0.05f, 1.0f, 0.05f, 1.0f);
		red = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	}
};

GUI_Console* console = nullptr;

ModuleEditor::ModuleEditor(const char* name, bool start_enabled) : Module(name, start_enabled)
{
	show_about = windows_hidden = show_imgui_demo = false;

	tests_menu = new GUI_Tests(false);
	config_menu = new GUI_Config(false);
	
	console = new GUI_Console(true);
	console->LogConsole("------------ Initializing Console -----------------\n");
	console->LogConsole("------------ Console Ready -----------------\n");

	colors = new Colors();
	
}

ModuleEditor::~ModuleEditor()
{
	std::list<GUI_Window*>::reverse_iterator i = editor_windows.rbegin();
	for (; i != editor_windows.rend(); ++i)
	{
		delete (*i);
	}

	delete colors;
}

bool ModuleEditor::Init()
{
	AddWindow(tests_menu);
	AddWindow(config_menu);
	AddWindow(console);

	colors->white = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	colors->Black = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	colors->yellow = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
	colors->skyBlue = ImVec4(0.10f, 0.10f, 1.00f, 1.0f);
	colors->blue = ImVec4(0.3f, 0.0f, 0.0f, 1.0f);
	colors->darkBlue = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	colors->pink = ImVec4(1.0f, 0.0f, 1.0f, 1.0f);
	colors->Green = ImVec4(0, 0.88f, 0.73f, 1.00f);
	colors->LighterGreen = ImVec4(0.5f, 1.0f, 0.5f, 1.0f);
	colors->DarkerGreen = ImVec4(0.05f, 0.4f, 0.05f, 1.0f);
	colors->AlphaGreen = ImVec4(0.05f, 1.0f, 0.05f, 1.0f);
	colors->red = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);

	style = &ImGui::GetStyle();
	
	//ImGui Styling
	style->WindowPadding = ImVec2(15, 15);
	style->WindowRounding = 5.0f;
	style->FramePadding = ImVec2(5, 5);
	style->FrameRounding = 4.0f;
	style->ItemSpacing = ImVec2(12, 8);
	style->ItemInnerSpacing = ImVec2(8, 6);
	style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 9.0f;
	style->GrabMinSize = 5.0f;
	style->GrabRounding = 3.0f;
	
	style->Colors[ImGuiCol_Text] = colors->white;
	//style->Colors[ImGuiCol_FrameBg] = colors.darkBlue;
	style->Colors[ImGuiCol_WindowBg] = colors->Black;
	style->Colors[ImGuiCol_MenuBarBg] = colors->DarkerGreen;
	style->Colors[ImGuiCol_ScrollbarGrab] = colors->DarkerGreen;
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = colors->LighterGreen;
	style->Colors[ImGuiCol_SliderGrab] = colors->LighterGreen;
	style->Colors[ImGuiCol_PlotHistogram] = colors->LighterGreen;
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_Header] = colors->DarkerGreen;
	style->Colors[ImGuiCol_CloseButton] = colors->pink;

	return true;
}

update_status ModuleEditor::Update(float dt)
{
	ImGui::BeginMainMenuBar();

	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("New"))
			return UPDATE_STOP;

		if (ImGui::MenuItem("Open"))
			return UPDATE_STOP;

		if (ImGui::MenuItem("Save"))
			return UPDATE_STOP;

		if (ImGui::MenuItem("Quit"))
			return UPDATE_STOP;

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("View"))
	{
		if (ImGui::MenuItem("Demo"))
			show_imgui_demo = !show_imgui_demo;

		if (ImGui::MenuItem("Tests"))
			tests_menu->ToggleActive();

		if (ImGui::MenuItem("Configuration"))
			config_menu->ToggleActive();

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Github"))
	{
		if (ImGui::MenuItem("Repository URL"))
			App->OpenURL("https://github.com/AhoyDev/Rubens-Engine");

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Help"))
	{
		if (ImGui::MenuItem("About"))
			show_about = !show_about;

		if (ImGui::MenuItem("Documentation"))
			App->OpenURL("https://github.com/AhoyDev/Rubens-Engine/wiki");

		if (ImGui::MenuItem("Download Latest"))
			App->OpenURL("https://github.com/AhoyDev/Rubens-Engine/releases");

		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();

	if (!windows_hidden)
		DrawWindows();

	//Pressing M hides the main menu for focused work
	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
		windows_hidden = !windows_hidden;

	return UPDATE_CONTINUE;
}

bool ModuleEditor::CleanUp()
{
	editor_windows.clear();
	return true;
}

void ModuleEditor::Draw()
{
	ImGui::Render();
}

void ModuleEditor::DrawWindows()
{
	std::list<GUI_Window*>::iterator i = editor_windows.begin();
	for (; i != editor_windows.end(); ++i)
	{
		(*i)->DrawWindow();
	}

	if (show_imgui_demo)
		ImGui::ShowTestWindow();

	if (show_about)
	{
		if (ImGui::Begin("_About"))
		{
			ImGui::TextColored(ImVec4(230, 250, 240, 255), "Engine Name:");
			ImGui::Text("R Engine");

			ImGui::TextColored(ImVec4(230, 250, 240, 255), "About: ");
			ImGui::Text("Engine made in CITM (UPC) to develop an RPG game");

			ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Core Developers");
			ImGui::TextColored(colors->Green, "Ruben Sardon");
			ImGui::TextColored(colors->yellow, "Ruben Idigora");

			ImGui::Separator();
			ImGui::TextColored(ImVec4(230, 250, 240, 255), "Libraries used");

			ImGui::Text("SDL");

			// SDL Versions
			/*SDL_version ver;
			SDL_VERSION(&ver);
			ImGui::Text("Compiled SDL version: %d.%d.%d", ver.major, ver.minor, ver.patch);
			SDL_GetVersion(&ver);
			ImGui::Text("Linked SDL version: %d.%d.%d", ver.major, ver.minor, ver.patch);
			ImGui::Text("SDL Revision: %s", SDL_GetRevision());
			ImGui::Separator();*/


			ImGui::Text("OpenGL");
			ImGui::Text("Imgui");
			ImGui::Text("MathGeoLib");
		}

		ImGui::End();
	}
}

void ModuleEditor::AddWindow(GUI_Window* win)
{
	editor_windows.push_back(win);
}