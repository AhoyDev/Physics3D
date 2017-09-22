#include "Application.h"

#include "Module.h"
#include "ModuleFS.h"
#include "ModuleTime.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModulePhysics3D.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleEditor.h"

#include "GUI_Console.h"

#include "Brofiler/include/Brofiler.h"

Application::Application()
{
	editor = new ModuleEditor();
	fs = new ModuleFS();
	time = new ModuleTime();
	window = new ModuleWindow();
	physics3D = new ModulePhysics3D();
	input = new ModuleInput();
	audio = new ModuleAudio();
	scene_intro = new ModuleSceneIntro();
	renderer3D = new ModuleRenderer3D();
	camera = new ModuleCamera3D();
	

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	//AddModule(fs);
	AddModule(editor);
	AddModule(time);
	AddModule(window);
	AddModule(input);
	AddModule(audio);
	AddModule(physics3D);
	AddModule(camera);

	AddModule(scene_intro);

	;
	AddModule(renderer3D);
}

Application::~Application()
{
	std::list<Module*>::reverse_iterator i = list_modules.rbegin();
	for (; i != list_modules.rend(); ++i)
	{
		delete (*i);
	}
}

bool Application::Init()
{
	bool ret = true;

	//char* buffer = nullptr;
	//App->fs->LoadFileToBuffer(&buffer, "Configuration.json");

	// Call Init() in all modules
	std::list<Module*>::iterator item = list_modules.begin();
	while (ret && item != list_modules.end())
	{
		ret = (*item)->Init();
		item++;
	}

	// After all Init calls we call Start() in all modules
	console->LogConsole("Application Start --------------\n");
	item = list_modules.begin();
	while (ret && item != list_modules.end())
	{
		if((*item)->IsEnabled())
			ret = (*item)->Start();

		item++;
	}
	
	return ret;
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	time->ManageFrameTimers();
	// manage events
	// check if needed resources must load for next frame
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	//BROFILER_FRAME("GameLoop")
	update_status ret = UPDATE_CONTINUE;

	float dt = time->UpdateDeltaTime();

	std::list<Module*>::iterator item = list_modules.begin();
	while (ret == UPDATE_CONTINUE && item != list_modules.end())
	{
		if ((*item)->IsEnabled())
			ret = (*item)->PreUpdate(dt);
		item++;
	}

	item = list_modules.begin();
	while (ret == UPDATE_CONTINUE && item != list_modules.end())
	{
		if ((*item)->IsEnabled())
			ret = (*item)->Update(dt);
		item++;
	}

	item = list_modules.begin();
	while (ret == UPDATE_CONTINUE && item != list_modules.end())
	{
		if ((*item)->IsEnabled())
			ret = (*item)->PostUpdate(dt);
		item++;
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	std::list<Module*>::reverse_iterator rit = list_modules.rbegin();
	for (; rit != list_modules.rend() && ret; ++rit)
	{
		ret = (*rit)->CleanUp();
	}

	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}


void Application::OpenURL(const char* url)
{
	ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
}
