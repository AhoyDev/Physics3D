#ifndef __ModuleUI_H__
#define __ModuleUI_H__

#include "Module.h"
#include <list>

class GUI_Window;
class EditorMainMenu;
class GUI_Tests;
class GUI_Config;
class ImGuiStyle;
struct Colors;

class ModuleEditor : public Module
{
public:

	ModuleEditor(const char* name, bool start_enabled = true);
	~ModuleEditor();

	bool Init(JSONNode config);
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	bool CleanUp();

	void Draw();

private:

	void DrawWindows();
	void AddWindow(GUI_Window* win);

public:

	//UI MENUS
	EditorMainMenu* main_menu;
	GUI_Tests* tests_menu;
	GUI_Config* config_menu;

	ImGuiStyle* style;
	Colors* colors;

private:

	bool show_imgui_demo;
	bool show_about;
	bool windows_hidden;
	std::list<GUI_Window*> editor_windows;
};



#endif // __ModuleAudio_H__