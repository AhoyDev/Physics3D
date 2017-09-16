#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <list>
#include "Globals.h"
#include "Timer.h"

class Module;
class ModuleWindow;
class ModuleInput;
class ModuleAudio;
class ModulePhysics3D;
class ModuleCamera3D;
class ModuleSceneIntro;
class ModuleEditor;
class ModuleRenderer3D;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleSceneIntro* scene_intro;
	ModulePhysics3D* physics3D;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleEditor* editor;
	
	/* Future Modules
	ModuleFileSystem* file_system;
	ModuleGOManager* go_manager;
	ModuleResourceManager* resource_manager;
	ModuleLighting* lighting;
	ModuleScripting* scripting;
	*/

private:

	Timer	ms_timer;
	Timer	fps_timer;
	Uint32	frames;
	float	dt;
	int		fps_counter;
	int		last_frame_ms;
	int		last_fps;
	int		capped_ms;
	std::list<Module*> list_modules;
};

extern Application* App;

#endif