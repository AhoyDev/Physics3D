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

public:

	std::vector<float> fps_plot_values;
};




#endif