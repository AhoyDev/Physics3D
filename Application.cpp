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
#include "GUI_Console.h"
#include "JSONNode.h"
#include "Globals.h"

#include "Brofiler/include/Brofiler.h"

Application::Application() :
	fm(nullptr), time(nullptr),
	window(nullptr), input(nullptr), audio(nullptr), physics3D(nullptr),
	camera(nullptr), scene_intro(nullptr), editor(nullptr), renderer3D(nullptr)
{
	fm = new WindowsFileManager();
	time = new TimeManager();

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
}

Application::~Application()
{
	std::list<Module*>::reverse_iterator i = list_modules.rbegin();
	for (; i != list_modules.rend(); ++i)
	{
		delete (*i);
	}

	if (fm) delete fm;
	if (time) delete time;
}

bool Application::Init()
{
	bool ret = true;

	char* buffer = nullptr;
	if (!fm->LoadFileToBuffer(&buffer, "Configuration.json")) // couldn't load config
	{
		LOG("Error while loading Configuration file. Creating new JSON stream\n");
		
		JSONNode json_node;

		std::list<Module*>::iterator i = list_modules.begin();
		for (; i != list_modules.end(); ++i)
			json_node.PushJObject((*i)->GetName());

		uint size = json_node.Serialize(&buffer);
		fm->Save("Configuration.json", buffer);
	}
	JSONNode config(buffer);
	delete[] buffer;

	// Call Init() in all modules
	std::list<Module*>::iterator item = list_modules.begin();
	for (; ret && item != list_modules.end(); item++)
	{
		ret = (*item)->Init(config.PullJObject((*item)->GetName()));
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------\n");
	item = list_modules.begin();
	for (; ret && item != list_modules.end(); item++)
	{
		if((*item)->IsEnabled())
			ret = (*item)->Start();
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
		LOG((*rit)->GetName());
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
