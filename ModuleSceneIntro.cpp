#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "GUI_Console.h"
ModuleSceneIntro::ModuleSceneIntro(bool start_enabled) : Module(start_enabled),
graphics(NULL), ground(NULL)
{}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	console->LogConsole("Loading Intro assets\n");
	bool ret = true;


	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	console->LogConsole("Unloading Intro scene\n");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update(float dt)
{
	PrimPlane(0, 1, 0, 0).Render();

	return UPDATE_CONTINUE;
}