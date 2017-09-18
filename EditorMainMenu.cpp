#include "Globals.h"
#include "Application.h"
#include "EditorMainMenu.h"
#include "imgui/imgui.h"


EditorMainMenu::EditorMainMenu()
{
	hardware_menu = new GUI_Hardware();
}

update_status EditorMainMenu::ShowMainMenu()
{
	ImGui::BeginMainMenuBar();

	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Quit"))
			return UPDATE_STOP;

		ImGui::EndMenu();
	}


	if (ImGui::BeginMenu("View"))
	{
		if (ImGui::MenuItem("HardWare"))
			configuration = !configuration;

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
		{
			ImGui::BeginPopup("Welcome to the best engine ever made. With it any game can be accomplished. ;)");
			ImGui::EndPopup();
		}


		if (ImGui::MenuItem("Demo"))
			demo = !demo;

		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();



	if (configuration)
		hardware_menu->ShowHardwareMenu();




	return UPDATE_CONTINUE;
}
