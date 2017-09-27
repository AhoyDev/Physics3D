#include "GUI_Console.h"

#include "Globals.h"
#include "imgui\imgui.h"

GUI_Console::GUI_Console(const bool active) : GUI_Window(active)
{
	buf = new ImGuiTextBuffer();
}

GUI_Console::~GUI_Console()
{
	delete buf;
}

void GUI_Console::LogConsole(const char* file)
{
	if (file)
	{
		buf->append(file);
		scroll_bot = true;
	}
	else
		LOG("message to be displayed NULL");
}

void GUI_Console::Log(const char* format, ...)
{
	static char content[4096];
	static va_list  ap;
	va_start(ap, format);
	vsprintf_s(content, 4096, format, ap);
	buf->append(content);
	scroll_bot = true;
}

void GUI_Console::Draw()
{
	ImGui::Begin("Console Log");
	ImGui::TextUnformatted(buf->begin());
	if (scroll_bot)
		ImGui::SetScrollHere(1.0f);
	scroll_bot = false;

	ImGui::End();
}
