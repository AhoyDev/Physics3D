#ifndef __GUICONFIG_H__
#define __GUICONFIG_H__

#include "GUI_Window.h"
#include <string>
#include <vector>

struct HardwareInfo
{
	
};

class GUI_Config : public GUI_Window
{
public:
	GUI_Config(const bool active = false);
	~GUI_Config();

	void Draw();

	void UpdateHardware();
	void UpdateVRAM();

private:

	void ShowApp();
	void ShowWindow();
	void ShowHardware();
	void ShowMemory();
	void ShowGLOptions();
	
	float AddPlotValue(std::vector<float> &vec, float value);

private:

	std::vector<float> fps;
	std::vector<float> ms;
	std::vector<float> mem;

	// Hardware
	std::string cpus;
	std::string ram;
	std::string caps1;
	std::string caps2;
	std::string drivers;
	std::string gpu_renderer;
	std::string gpu_vendor;
	std::string gpu_version;
	std::string gpu_shading;
	std::string vram_total;
	std::string vram_used;
	std::string vram_available;
	std::string vram_reserved;
};


#endif