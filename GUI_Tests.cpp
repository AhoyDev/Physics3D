#include "GUI_Tests.h"
#include "imgui\imgui.h"

GUI_Tests::GUI_Tests()
{
	flags |= ImGuiWindowFlags_AlwaysVerticalScrollbar;
}

void GUI_Tests::ShowIntersectionMenu()
{
	
	bool p = true;

	
	ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Intersection Tests",&p, ImGuiWindowFlags_AlwaysVerticalScrollbar);

	if (ImGui::CollapsingHeader("Test 1"))
	{
		ImGui::TextWrapped("Intersection between Sphere and cilinder");
		
	}

	if (ImGui::CollapsingHeader("Test 2"))
	{
		ImGui::TextWrapped("Intersection between Sphere and cilinder");

	}
	if (ImGui::CollapsingHeader("Test 3"))
	{
		ImGui::TextWrapped("Intersection between Sphere and cilinder");

	}
	if (ImGui::CollapsingHeader("Test 4"))
	{
		ImGui::TextWrapped("Intersection between Sphere and cilinder");

	}



	ImGui::End();
}
