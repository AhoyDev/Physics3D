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

	ModuleEditor(bool start_enabled = true);
	~ModuleEditor();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	bool CleanUp();

private:
	EditorMainMenu* main_menu;
	RandomGenerator* random_machine;
	



	R_Sphere* sphere;
	R_Capsule* capsule;

};

#endif // __ModuleAudio_H__