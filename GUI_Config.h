#ifndef __GUICONFIG_H__
#define __GUICONFIG_H__

#include "MathGeoLib\include\MathGeoLib.h"
#include <vector>
#include "imgui\imgui.h"
#include "Globals.h"

class GUI_Config
{
public:
	GUI_Config();
	~GUI_Config();

	void ShowConfigMenu();

private:

	void ShowApp();
	void ShowWindow();
	void ShowHardware();
	void ShowMemory();
	void ShowGLOptions();
public:

	std::vector<float> fps;
	std::vector<float> ms;
	std::vector<float> memory;

	Colors colors;
};




#endif