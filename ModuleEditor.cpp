#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"

/*#include "Glew\include\glew.h"
//#include <GL/gl3w.h>*/

#include "imgui/imgui.h"
#include "Imgui\imgui_impl_sdl_gl3.h"


ModuleEditor::ModuleEditor(Application * app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleEditor::~ModuleEditor()
{
	
}

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

	ImGui_ImplSdlGL3_NewFrame(App->window->window);

	/*ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize.x = 1920.0f;
	io.DisplaySize.y = 1280.0f;
	io.IniFilename = "imgui.ini";


	unsigned char* pixels;
	int width, height;

	//io.Fonts->AddFontDefault();

	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

	ImGui::NewFrame();*/

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
