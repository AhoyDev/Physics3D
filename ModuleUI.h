#ifndef __ModuleUI_H__
#define __ModuleUI_H__

#include "Module.h"


class ModuleUI : public Module
{
public:

	ModuleUI(Application* app, bool start_enabled = true);
	~ModuleUI();

	bool Init();
	update_status PreUpdate(float dt);
	bool CleanUp();


};

#endif // __ModuleAudio_H__