#include "Application.h"
#include "Globals.h"
#include "ModuleEditor.h"
#include "SDL\include\SDL.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "imgui\imgui.h"
#include "EditorMainMenu.h"
#include "GUI_Config.h"
#include "RandomGenerator.h"


#include "MathGeoLib\include\MathGeoLib.h"




ModuleEditor::ModuleEditor(const char* name, bool start_enabled) : Module(name, start_enabled)
{
	main_menu = new EditorMainMenu();
	tests_menu = new GUI_Tests();
	config = new GUI_Config();
	
}

ModuleEditor::~ModuleEditor()
{

}

bool ModuleEditor::Init(JSONNode config)
{
	
	random_machine = new RandomGenerator();
	console = new GUI_Console();

	console->LogConsole("------------ Initializing Console -----------------\n");
	console->LogConsole("------------ Console Ready -----------------\n");

	Colors colors;
	
	//ImGui Styling
	style.WindowPadding = ImVec2(15, 15);
	style.WindowRounding = 5.0f;
	style.FramePadding = ImVec2(5, 5);
	style.FrameRounding = 4.0f;
	style.ItemSpacing = ImVec2(12, 8);
	style.ItemInnerSpacing = ImVec2(8, 6);
	style.IndentSpacing = 25.0f;
	style.ScrollbarSize = 15.0f;
	style.ScrollbarRounding = 9.0f;
	style.GrabMinSize = 5.0f;
	style.GrabRounding = 3.0f;
	

	style.Colors[ImGuiCol_Text] = colors.white;
	//style.Colors[ImGuiCol_FrameBg] = colors.darkBlue;
	style.Colors[ImGuiCol_WindowBg] = colors.Black;
	style.Colors[ImGuiCol_MenuBarBg] = colors.DarkerGreen;
	style.Colors[ImGuiCol_ScrollbarGrab] = colors.DarkerGreen;
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = colors.LighterGreen;
	style.Colors[ImGuiCol_SliderGrab] = colors.LighterGreen;
	style.Colors[ImGuiCol_PlotHistogram] = colors.LighterGreen;
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_Header] = colors.DarkerGreen; 
	style.Colors[ImGuiCol_CloseButton] = colors.pink;

	return true;
}

update_status ModuleEditor::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update(float dt)
{
	update_status status = UPDATE_CONTINUE;

	//Bool that handles imgui demo
	if(main_menu->demo)
		ImGui::ShowTestWindow();
	
	//If quit it returns UPDATE_STOP
	
	if(main_menu->show)
		status = main_menu->ShowMainMenu();

	if (tests)
		tests_menu->ShowIntersectionMenu();

	if (configuration)
		config->ShowConfigMenu();


	//Pressing M hides the main menu for focused work

	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
	{
		if (main_menu->show)
			main_menu->show = false;
		else
			main_menu->show = true;

		if (tests)
			tests = !tests;

		if (configuration)
			configuration = !configuration;
		
	}

	//LOG("Random Number 1-100 = %d ", random_machine->RandomInt(0,100));
	console->DrawLineConsole();


	return status;
}

bool ModuleEditor::CleanUp()
{
	main_menu->CleanUp();
	delete main_menu;
	delete random_machine;
	delete console;
	return true;
}
