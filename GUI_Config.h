#ifndef __GUICONFIG_H__
#define __GUICONFIG_H__

#include "GUI_Window.h"
#include <vector>

class GUI_Config : public GUI_Window
{
public:
	GUI_Config(const bool active = false);
	~GUI_Config();

	void Draw();

private:

	void ShowApp();
	void ShowWindow();
	void ShowHardware();
	void ShowMemory();
	void ShowGLOptions();

private:

	std::vector<float> fps;
	std::vector<float> ms;
	std::vector<float> mem;
};


#endif