#pragma once
#include "imgui/imgui.h"
#include "GUI_Hardware.h"
#include "GUI_Tests.h"

//TOP BAR MAIN MENU (FILE, VIEW ETC..)
class EditorMainMenu 
{
public:

	EditorMainMenu();

	update_status ShowMainMenu();

	void CleanUp();

private:
	GUI_Hardware* hardware_menu;
	GUI_Tests* tests_menu;

public:
	bool demo = false;
	bool show = true;
	bool configuration = false;
	bool tests = false;
};




