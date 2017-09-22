#include "Globals.h"
#include "GUI_Tests.h"
#include "imgui\imgui.h"

#include "Primitives.h"
#include "R_Sphere.h"

#include "RandomGenerator.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include "ModuleEditor.h"




GUI_Tests::GUI_Tests()
{
	flags |= ImGuiWindowFlags_AlwaysVerticalScrollbar;

	random_machine = new RandomGenerator();

}

//Test sphere with
void GUI_Tests::Test1()
{
	vec posCapsule = vec((float)random_machine->RandomInt(0, 1), (float)random_machine->RandomInt(0, 1), (float)random_machine->RandomInt(0, 1));
	
	vec dirCapsule = vec((float)random_machine->RandomInt(0, 1), (float)random_machine->RandomInt(0, 1), (float)random_machine->RandomInt(0, 1));
	Line lineCapsule = Line(posCapsule, dirCapsule);

	float capsule_direction = random_machine->RandomInt(0, 20);

	LineSegment capsule_segment = LineSegment(lineCapsule,capsule_direction);

	float capsule_radius = random_machine->RandomInt(0, 5);

	capsule = new R_Capsule(capsule_segment,capsule_radius);
	




	vec posSphere = vec(random_machine->RandomInt(0, 20), random_machine->RandomInt(0, 20), random_machine->RandomInt(0, 20));

	float radiusSphere = random_machine->RandomInt(0, 20);


	sphere = new R_Sphere(posSphere,radiusSphere);


	
	bool result_intersection = sphere->Intersects(*capsule);

	console->LogConsole("The result of the intersection between a Sphere and a Capsule is: %d (1 = intersects, 0 = doesn't intersect)\n");
	

}

void GUI_Tests::Test2()
{

}

void GUI_Tests::Test3()
{

}

void GUI_Tests::Test4()
{

}

void GUI_Tests::ShowIntersectionMenu()
{
	
	bool p = false;

	
	ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Intersection Tests",&p, ImGuiWindowFlags_AlwaysVerticalScrollbar);

	if (ImGui::CollapsingHeader("Test 1"))
	{
		ImGui::TextWrapped("Intersection between Sphere and cilinder");
		
		static bool a = false;
		if (ImGui::Button("1 Test"))
		{	
			LOG("Clicked"); 
			a = !a; 
		}
		if (a)
		{
			Test1();
			a = !a;
		}
	
	}
	
	if (ImGui::CollapsingHeader("Test 2"))
	{
		ImGui::TextWrapped("Intersection between Capsule and Sphere");
		Test2();
	}
	if (ImGui::CollapsingHeader("Test 3"))
	{
		ImGui::TextWrapped("Intersection between Sphere and cilinder");
		Test3();
	}
	if (ImGui::CollapsingHeader("Test 4"))
	{
		ImGui::TextWrapped("Intersection between Sphere and cilinder");
		Test4();
	}



	ImGui::End();
}
