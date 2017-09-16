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

Application::Application()
{
	frames = 0;
	last_frame_ms = -1;
	last_fps = -1;
	capped_ms = 1000 / 60;
	fps_counter = 0;

	window = new ModuleWindow();
	physics3D = new ModulePhysics3D();
	input = new ModuleInput();
	audio = new ModuleAudio();
	scene_intro = new ModuleSceneIntro();
	renderer3D = new ModuleRenderer3D();
	camera = new ModuleCamera3D();
	editor = new ModuleEditor();

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

	/* STDSUB POLISH
	p2List_item<Module*>* item = list_modules.getLast();

	while(item != NULL)
	{
		delete item->data;
		item = item->prev;
	}*/
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	std::list<Module*>::iterator item = list_modules.begin();
	while (ret && item != list_modules.end())
	{
		ret = (*item)->Init();
		item++;
	}

	/* STDSUB POLISH
	p2List_item<Module*>* item = list_modules.getFirst();

	while(item != NULL && ret == true)
	{
		ret = item->data->Init();
		item = item->next;
	}*/


	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	item = list_modules.begin();
	while (ret && item != list_modules.end())
	{
		if((*item)->IsEnabled())
			ret = (*item)->Start();

		item++;
	}

	/* STDSUB POLISH
	item = list_modules.getFirst();

	while(item != NULL && ret == true)
	{
		if(item->data->IsEnabled())
			ret = item->data->Start();
		item = item->next;
	}*/
	
	return ret;
}


// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	// Recap on framecount and fps
	++frames;
	++fps_counter;

	if(fps_timer.Read() >= 1000)
	{
		last_fps = fps_counter;
		fps_counter = 0;
		fps_timer.Start();
	}

	last_frame_ms = ms_timer.Read();

	// cap fps
	if(last_frame_ms < capped_ms)
	{
		SDL_Delay(capped_ms - last_frame_ms);
	}

	//char t[50];
	//sprintf_s(t, "FPS: %d", (int)last_fps);
	//window->SetTitle(t);
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();


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

	/* STDSUB POLISH
	p2List_item<Module*>* item = list_modules.getFirst();
	
	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		if(item->data->IsEnabled())
			ret = item->data->PreUpdate(dt);
		item = item->next;
	}

	item = list_modules.getFirst();

	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		if(item->data->IsEnabled())
  			ret = item->data->Update(dt);
		item = item->next;
	}

	item = list_modules.getFirst();

	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		if(item->data->IsEnabled())
			ret = item->data->PostUpdate(dt);
		item = item->next;
	}*/

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

	/* STDSUB POLISH
	p2List_item<Module*>* item = list_modules.getLast();

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}*/

	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}