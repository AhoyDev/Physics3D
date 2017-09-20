#ifndef __GUICONSOLE_H__
#define __GUICONSOLE_H__

#include "imgui\imgui.h"

class GUI_Console
{
public:
	GUI_Console();


private:
	

public:
	void LogConsole(const char* file);
	void DrawLineConsole();

	ImGuiTextBuffer		buf;
	ImGuiTextFilter		filter;


private:
	bool scroll_bot;
};
extern GUI_Console* console;


#endif
