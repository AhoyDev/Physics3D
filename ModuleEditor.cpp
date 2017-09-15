#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"

#include "imgui/imconfig.h"
#include "imgui/imgui.h"



//Connect to App, ShowInfoPanel in Update method

#pragma comment (lib, "Glew/libx86/glew32.lib")

ModuleEditor::ModuleEditor(Application * app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleEditor::~ModuleEditor()
{
	
}

bool ModuleEditor::Init()
{

	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize.x = 1920.0f;
	io.DisplaySize.y = 1280.0f;
	io.IniFilename = "imgui.ini";
	
	unsigned char* pixels;
	int width, height;

	io.Fonts->AddFontDefault();

	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

	return true;
}

update_status ModuleEditor::PreUpdate(float dt)
{
	ImGui::NewFrame();

	ImGui::ShowTestWindow();


	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleEditor::CleanUp()
{
	return true;
}
