#include "Globals.h"
#include "Application.h"
#include "EditorMainMenu.h"
#include "imgui/imgui.h"

#include "GUI_Config.h"

EditorMainMenu::EditorMainMenu()
{
	
	tests_menu = new GUI_Tests();
	config = new GUI_Config();
	
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

		if (ImGui::MenuItem("Tests"))
			tests = !tests;

		if (ImGui::MenuItem("Configuration"))
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
			about = !about;
		}
		

		if (ImGui::MenuItem("Demo"))
			demo = !demo;

		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();


	//Shows hardware if activated on the main menu


	if (tests)
		tests_menu->ShowIntersectionMenu();
	if (configuration)
		config->ShowConfigMenu();
	
	if(about)
	{
		if (ImGui::Begin("_About"))
		{	
			ImGui::TextColored(ImVec4(230,250,240,255), "Engine Name:");
			ImGui::Text("R Engine");

			ImGui::TextColored(ImVec4(230, 250, 240, 255), "About: ");
			ImGui::Text("Engine made in CITM (UPC) to develop an RPG game");

			ImGui::TextColored(ImVec4(230, 250, 240, 255), "Core Developers: ");
			ImGui::Text("Ruben Sardon");
			ImGui::Text("Ruben Idigora");

			ImGui::TextColored(ImVec4(230, 250, 240, 255), "Libraries used");

			ImGui::Text("SDL");
			ImGui::Text("OpenGL");
			ImGui::Text("Imgui");
			ImGui::Text("MathGeoLib");
		}
		ImGui::End();
	}



	return UPDATE_CONTINUE;
}

void EditorMainMenu::CleanUp()
{
	delete tests_menu;

}
