#ifndef __GUICONFIG_H__
#define __GUICONFIG_H__

#include "GUI_Window.h"
#include <vector>

struct ConfigValues
{
	int config_max_fps, max_fps;
	int config_width, width;
	int config_height, height;
	bool config_vsync, vsync;
	bool config_fullScreen, fullScreen;
	bool config_resizable, resizable;
	bool config_borderless, borderless;
	bool config_fullscreenDesktop, fullscreenDesktop;

	bool NeedSaving();
	bool NeedRestart();
};

class GUI_Config : public GUI_Window
{
public:
	GUI_Config(const bool active = false);
	~GUI_Config();

	void Draw();
	void SetConfigValues();

private:

	void ShowApp();
	void ShowWindow();
	void ShowHardware();
	void ShowMemory();
	void ShowGLOptions();

private:

	ConfigValues values;

	std::vector<float> fps;
	std::vector<float> ms;
	std::vector<float> mem;
};


#endif