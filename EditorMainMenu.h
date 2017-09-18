#pragma once
#include "imgui/imgui.h"
#include "GUI_Hardware.h"

class EditorMainMenu 
{
public:

	EditorMainMenu();

	update_status ShowMainMenu();

	

private:
	GUI_Hardware* hardware_menu;
public:
	bool demo = false;
	bool show = true;
	bool configuration = false;

};




