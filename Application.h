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
class RandomGenerator;

class JSONNode;

struct ConfigValues
{
	const char *config_title, *title;
	int config_max_fps, max_fps;
	int config_width, width;
	int config_height, height;
	bool config_fullScreen, fullScreen;
	bool config_resizable, resizable;
	bool config_borderless, borderless;
	bool config_fullscreenDesktop, fullscreenDesktop;
	bool config_vsync, vsync;

	bool NeedSaving();
	bool NeedRestart();
};

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	update_status Restart();

	void RequestRestart();
	void RequestSave();
	void OpenURL(const char* url);

private: 

	void SetConfig();
	void SetConfigValues();
	void Save() const;
	void Load();
	void FinishUpdate();

	void AddModule(Module* mod);

public:

	FileManager* fm;
	TimeManager* time;
	RandomGenerator* rnd;

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

	ConfigValues config_values;

private:

	bool request_restart;
	bool request_save;
	JSONNode* config;
	std::list<Module*> list_modules;
	
};

extern Application* App;

#endif