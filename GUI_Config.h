#ifndef __GUICONFIG_H__
#define __GUICONFIG_H__

#include "MathGeoLib\include\MathGeoLib.h"
#include <deque>



class GUI_Config
{
public:
	GUI_Config();

	void ShowConfigMenu();


	std::deque<float> fps_plot_values;
};




#endif