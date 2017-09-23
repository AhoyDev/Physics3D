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

#include "JSONNode.h"

#include "Brofiler/include/Brofiler.h"

Application::Application()
{
	fs = new ModuleWindowsFS("File System");
	time = new ModuleTime("Time");
	editor = new ModuleEditor("Editor");
	window = new ModuleWindow("Window");
	input = new ModuleInput("Input");
	audio = new ModuleAudio("Audio");
	physics3D = new ModulePhysics3D("Physics");
	camera = new ModuleCamera3D("Camera");
	scene_intro = new ModuleSceneIntro("Scene Intro");
	renderer3D = new ModuleRenderer3D("Render");
	

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(fs);
	AddModule(time);
	AddModule(editor);
	AddModule(window);
	AddModule(input);
	AddModule(audio);
	AddModule(physics3D);
	AddModule(camera);

	AddModule(scene_intro);

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

	char* buffer = nullptr;
	fs->LoadFileToBuffer(&buffer, "Configuration.json");
	JSONNode config(buffer);
	delete[] buffer;

	// Call Init() in all modules
	std::list<Module*>::iterator item = list_modules.begin();
	while (ret && item != list_modules.end())
	{
		ret = (*item)->Init(config.PullJObject((*item)->GetName()));
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
