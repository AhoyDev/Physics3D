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

class FileManager;
class TimeManager;

class JSONNode;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	void OpenURL(const char* url);

private: 

	void SetConfig();
	void FinishUpdate();

	void Save() const;
	void Load() const;

	void AddModule(Module* mod);

public:

	FileManager* fm;
	TimeManager* time;

	// Modules
	ModuleWindow* window;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleSceneIntro* scene_intro;
	ModulePhysics3D* physics3D;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleEditor* editor;
	
	/* Future Modules
	ModuleGOManager* go_manager;
	ModuleResourceManager* resource_manager;
	ModuleLighting* lighting;
	ModuleScripting* scripting;*/

private:
	
	JSONNode* config;
	std::list<Module*> list_modules;
	
};

extern Application* App;

#endif