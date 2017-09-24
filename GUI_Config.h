#ifndef __GUICONFIG_H__
#define __GUICONFIG_H__

#include "MathGeoLib\include\MathGeoLib.h"
#include <vector>

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

public:

	std::vector<float> fps;
	std::vector<float> ms;
};




#endif