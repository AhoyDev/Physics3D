#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <list>
#include "Globals.h"

class Module;
class ModuleTime;
class ModuleWindow;
class ModuleInput;
class ModuleAudio;
class ModulePhysics3D;
class ModuleCamera3D;
class ModuleSceneIntro;
class ModuleEditor;
class ModuleRenderer3D;

class Timer;

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
	void FinishUpdate();

public:

	ModuleTime* time;
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
	
	std::list<Module*> list_modules;
};

extern Application* App;

#endif