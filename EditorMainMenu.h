#ifndef __EDITORMAINMENU_H__
#define __EDITORMAINMENU_H__

#include "Globals.h"
#include "imgui/imgui.h"
#include "GUI_Hardware.h"
#include "GUI_Tests.h"

class GUI_Config;
//TOP BAR MAIN MENU (FILE, VIEW ETC..)
class EditorMainMenu 
{
public:

	EditorMainMenu();

	update_status ShowMainMenu();

	void CleanUp();

private:
	
	GUI_Tests* tests_menu;
	

public:
	bool demo = false;
	bool show = true;
	bool about = false;
	bool hardware = false;
	
	GUI_Config* config;
	Colors colors;
};
#endif



