#include "Globals.h"
#include "Application.h"
#include "EditorMainMenu.h"
#include "imgui/imgui.h"
#include "ModuleEditor.h"
#include "GUI_Config.h"

EditorMainMenu::EditorMainMenu()
{
	
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
			App->editor->tests = !App->editor->tests;

		if (ImGui::MenuItem("Configuration"))
			App->editor->configuration = !App->editor->configuration;


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
		
		if (ImGui::MenuItem("Documentation"))
		{
			App->OpenURL("https://github.com/AhoyDev/Rubens-Engine/wiki");
		}
		if (ImGui::MenuItem("Download Latest"))
		{
			App->OpenURL("https://github.com/AhoyDev/Rubens-Engine/releases");
		}

		if (ImGui::MenuItem("Demo"))
			demo = !demo;

		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();

	
	if(about)
	{
		if (ImGui::Begin("_About"))
		{	
			ImGui::TextColored(ImVec4(230,250,240,255), "Engine Name:");
			ImGui::Text("R Engine");

			ImGui::TextColored(ImVec4(230, 250, 240, 255), "About: ");
			ImGui::Text("Engine made in CITM (UPC) to develop an RPG game");

			
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Core Developers");
			ImGui::TextColored(colors.Green, "Ruben Sardon");
			ImGui::TextColored(colors.yellow, "Ruben Idigora");
	

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
	
}
