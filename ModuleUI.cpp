#include "Globals.h"
#include "Application.h"
#include "ModuleUI.h"

#include "imgui/imconfig.h"
#include "imgui/imgui.h"



//Connect to App, ShowInfoPanel in Update method

#pragma comment (lib, "Glew/libx86/glew32.lib")

ModuleUI::ModuleUI(Application * app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleUI::~ModuleUI()
{
	
}

bool ModuleUI::Init()
{
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize.x = 1920.0f;
	io.DisplaySize.y = 1280.0f;
	io.IniFilename = "imgui.ini";
	

	return false;
}

update_status ModuleUI::PreUpdate(float dt)
{
	return update_status();
}

bool ModuleUI::CleanUp()
{
	return false;
}
