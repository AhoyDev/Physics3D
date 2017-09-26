#include "Globals.h"
#include "GUI_Console.h"

GUI_Console* console;

GUI_Console::GUI_Console()
{
}

void GUI_Console::LogConsole(const char* file)
{
	if (file)
	{
		buf.append(file);
		scroll_bot = true;
	}
	else
		LOG("message to be displayed NULL");
}

void GUI_Console::DrawLineConsole()
{
	ImGui::Begin("Console Log");
	ImGui::TextUnformatted(buf.begin());
	if (scroll_bot)
		ImGui::SetScrollHere(1.0f);
	scroll_bot = false;

	ImGui::End();


}
