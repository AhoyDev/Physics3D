#include "Application.h"

#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModulePhysics3D.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleEditor.h"

#include "FileManager.h"
#include "TimeManager.h"
#include "RandomGenerator.h"
#include "GUI_Console.h"
#include "JSONNode.h"
#include "Globals.h"
#include "GUI_Config.h"

#include "Brofiler/include/Brofiler.h"

Application::Application() :
	fm(nullptr), time(nullptr),
	window(nullptr), input(nullptr), audio(nullptr), physics3D(nullptr),
	camera(nullptr), scene_intro(nullptr), editor(nullptr), renderer3D(nullptr)
{
	request_restart = request_save = false;

	fm = new WindowsFileManager();
	time = new TimeManager();
	rnd = new RandomGenerator();

	// Modules
	window = new ModuleWindow("Window");
	input = new ModuleInput("Input");
	audio = new ModuleAudio("Audio");
	physics3D = new ModulePhysics3D("Physics");
	camera = new ModuleCamera3D("Camera");
	scene_intro = new ModuleSceneIntro("Scene Intro");
	editor = new ModuleEditor("Editor");
	renderer3D = new ModuleRenderer3D("Render");

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(input);
	AddModule(audio);
	AddModule(physics3D);
	AddModule(camera);

	AddModule(scene_intro);

	AddModule(editor);
	AddModule(renderer3D);

	SetConfig();
}

Application::~Application()
{
	/*delete fm;
	delete time;
	delete config;*/

	std::list<Module*>::reverse_iterator i = list_modules.rbegin();
	for (; i != list_modules.rend(); ++i)
	{
		delete (*i);
	}
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	std::list<Module*>::iterator item = list_modules.begin();
	for (; ret && item != list_modules.end(); item++)
	{
		ret = (*item)->Init(config->PullJObject((*item)->GetName()));
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------\n");
	item = list_modules.begin();
	for (; ret && item != list_modules.end(); item++)
	{
		if((*item)->IsEnabled()) ret = (*item)->Start();
	}
	
	return ret;
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	//BROFILER_FRAME("GameLoop")
	update_status ret = UPDATE_CONTINUE;

	if (request_restart)
	{
		request_restart = false;
		ret = UPDATE_RESTART;
	}
	else
	{
		float dt = time->UpdateDeltaTime();

		std::list<Module*>::iterator item = list_modules.begin();
		for (; ret == UPDATE_CONTINUE && item != list_modules.end(); item++)
		{
			if ((*item)->IsEnabled())
				ret = (*item)->PreUpdate(dt);
		}

		item = list_modules.begin();
		for (; ret == UPDATE_CONTINUE && item != list_modules.end(); item++)
		{
			if ((*item)->IsEnabled())
				ret = (*item)->Update(dt);
		}

		item = list_modules.begin();
		for (; ret == UPDATE_CONTINUE && item != list_modules.end(); item++)
		{
			if ((*item)->IsEnabled())
				ret = (*item)->PostUpdate(dt);
		}

		FinishUpdate();
	}
	
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

update_status Application::Restart()
{
	update_status ret = UPDATE_STOP;

	if(CleanUp() && Init())
		ret = UPDATE_CONTINUE;

	return ret;
}

void Application::RequestRestart()
{
	request_restart = true;
}

void Application::RequestSave()
{
	request_save = true;
}

void Application::OpenURL(const char* url)
{
	ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
}

void Application::SetConfig()
{
	char* buffer = nullptr;

	if (!fm->LoadFileToBuffer(&buffer, "Configuration.json")) // couldn't load config
	{
		LOG("Error while loading Configuration file. Creating new JSON stream\n");

		config = new JSONNode();

		std::list<Module*>::iterator i = list_modules.begin();
		for (; i != list_modules.end(); ++i)
			config->PushJObject((*i)->GetName());

		uint size = config->Serialize(&buffer);
		fm->Save("Configuration.json", buffer);
	}
	else
	{
		config = new JSONNode(buffer);
	}

	delete[] buffer;
}

void Application::FinishUpdate()
{
	time->ManageFrameTimers();

	if (request_save)
	{
		request_save = false;
		Save();
		Load();
		editor->config_menu->SetConfigValues();
	}

	// manage events
	// check if needed resources must load for next frame
}

void Application::Save() const
{
	std::list<Module*>::const_iterator item = list_modules.begin();
	for (; item != list_modules.end(); item++)
	{
		(*item)->Save(&config->PullJObject((*item)->GetName()));
	}

	char* buffer = nullptr;
	uint size = config->Serialize(&buffer);
	fm->Save("Configuration.json", buffer);
	delete[] buffer;
}

void Application::Load()
{
	std::list<Module*>::const_iterator item = list_modules.begin();
	for (; item != list_modules.end(); item++)
	{
		(*item)->Load(&config->PullJObject((*item)->GetName()));
	}
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}



