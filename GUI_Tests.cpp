#include "GUI_Tests.h"

#include "imgui\imgui.h"
#include "Application.h"
#include "RandomGenerator.h"
#include "GUI_Console.h"
#include "SSegment.h"
#include "SCapsule.h"
#include "SSphere.h"

GUI_Tests::GUI_Tests(const bool active) : GUI_Window(active)
{}

//Test sphere with capsule
void GUI_Tests::Test()
{
	vec posCapsule = vec((float)App->rnd->RandomInt(0, 1), (float)App->rnd->RandomInt(0, 1), (float)App->rnd->RandomInt(0, 1));
	vec dirCapsule = vec((float)App->rnd->RandomInt(0, 1), (float)App->rnd->RandomInt(0, 1), (float)App->rnd->RandomInt(0, 1));
	float capsule_direction = App->rnd->RandomInt(0, 20);
	float capsule_radius = App->rnd->RandomInt(0, 5);

	SSegment capsule_segment = SSegment(posCapsule, posCapsule * capsule_radius);
	SCapsule capsule = SCapsule(capsule_segment,capsule_radius);

	vec posSphere = vec(App->rnd->RandomInt(0, 20), App->rnd->RandomInt(0, 20), App->rnd->RandomInt(0, 20));
	float radiusSphere = App->rnd->RandomInt(0, 20);

	SSphere sphere = SSphere(posSphere,radiusSphere);

	if(sphere.Intersects(&capsule))
		console->LogConsole("The result of the intersection between a Sphere and a Capsule is: TRUE");
	else
		console->LogConsole("The result of the intersection between a Sphere and a Capsule is: FALSE");
}

void GUI_Tests::Draw()
{
	ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiSetCond_FirstUseEver);

	ImGui::Begin("Intersection Tests");
	ImGui::TextWrapped("Intersection between Sphere and cilinder");

	if (ImGui::Button("1 Test"))
	{
		Test();
	}

	ImGui::End();
}
