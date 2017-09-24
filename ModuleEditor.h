#ifndef __ModuleUI_H__
#define __ModuleUI_H__

#include "Module.h"
#include "Globals.h"

#include "EditorMainMenu.h"
#include "GUI_Hardware.h"
#include "RandomGenerator.h"
#include "GUI_Console.h"

#include "Primitives.h"



class ModuleEditor : public Module
{
public:

	ModuleEditor(const char* name, bool start_enabled = true);
	~ModuleEditor();

	bool Init(JSONNode config);
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	bool CleanUp();


private:
	//UI MENUS
	EditorMainMenu* main_menu;
	GUI_Tests* tests_menu;
	GUI_Config* config;
public:
	//Bools for showing UI
	bool configuration = false;
	bool tests = false;



	RandomGenerator* random_machine;
	



	R_Sphere* sphere;
	R_Capsule* capsule;

public:
	ImGuiStyle& style = ImGui::GetStyle();
};

#endif // __ModuleAudio_H__