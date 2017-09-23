#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "Gui_Console.h"

ModulePlayer::ModulePlayer(const char* name, bool start_enabled) : Module(name, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	console->LogConsole("Loading player\n");

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	console->LogConsole("Unloading player\n");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	return UPDATE_CONTINUE;
}



