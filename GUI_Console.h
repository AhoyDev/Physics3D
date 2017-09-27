#ifndef __GUICONSOLE_H__
#define __GUICONSOLE_H__

#include "GUI_Window.h"

class ImGuiTextBuffer;

class GUI_Console : public GUI_Window
{
public:
	GUI_Console(const bool active = false);
	~GUI_Console();

	void Draw();
	void LogConsole(const char* file);
	void Log(const char* format, ...);

private:

	ImGuiTextBuffer*	buf;
	bool scroll_bot;
};

extern GUI_Console* console;

#endif
