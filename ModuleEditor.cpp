#include "ModuleEditor.h"

#include "Application.h"
#include "ModuleWindow.h"

#include "imgui\imgui.h"


ModuleEditor::ModuleEditor(bool start_enabled) : Module(start_enabled)
{}

ModuleEditor::~ModuleEditor()
{}

bool ModuleEditor::Init()
{
	//glewInit();

	/*ImGuiIO& io = ImGui::GetIO();
	ImFont* pFont = io.Fonts->AddFontFromFileTTF("imgui/extra_fonts/Cousine-Regular.ttf", 10.0f);

	unsigned char * pixels;
	int width, height, bytes_per_pixels;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height, &bytes_per_pixels);*/

	/*GLuint id = loadTexture(pixels, width, height, 4);

	io.Fonts->SetTexID((void*)id);*/

	return true;
}

update_status ModuleEditor::PreUpdate(float dt)
{
	LOG("EDITOR");

	/*ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize.x = 1920.0f;
	io.DisplaySize.y = 1280.0f;
	io.IniFilename = "imgui.ini";


	unsigned char* pixels;
	int width, height;

	//io.Fonts->AddFontDefault();

	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

	ImGui::NewFrame();*/

	


	return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update(float dt)
{
	ImGui::ShowTestWindow();

	return UPDATE_CONTINUE;
}

bool ModuleEditor::CleanUp()
{
	return true;
}
