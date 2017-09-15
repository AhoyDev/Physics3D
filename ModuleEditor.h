#ifndef __ModuleUI_H__
#define __ModuleUI_H__

#include "Module.h"


class ModuleEditor : public Module
{
public:

	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();


};

#endif // __ModuleAudio_H__