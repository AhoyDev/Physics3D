#include "ModuleRenderer3D.h"

#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleCamera3D.h"
#include "ModuleEditor.h"
#include "GUI_Console.h"
#include "Glew\include\glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/libx86/32bits/glew32.lib") 

#include "Imgui\imgui.h"
#include "Imgui\imgui_impl_sdl_gl3.h"

#include "Brofiler/include/Brofiler.h"

// TEMPORAL
#include "glmath.h"


ModuleRenderer3D::ModuleRenderer3D(const char* name, bool start_enabled) : Module(name, start_enabled)
{}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init(JSONNode config)
{
	console->LogConsole("Creating 3D Renderer context\n");
	bool ret = true;
	
	//Set attributes for SDL_GL
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	if (GLEW_OK != glewInit())
	{
		console->LogConsole("Glew failed\n");
		LOG("Using Glew %s", glewGetString(GLEW_VERSION));
	}

	if(ret == true)
	{
		if(App->config_values.vsync ? SDL_GL_SetSwapInterval(1) : SDL_GL_SetSwapInterval(0) == -1)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);
	}

	// Projection matrix for
	OnResize(App->window->GetWidth(), App->window->GetHeight());

	// Initialize ImGui w/ SdlGL3
	ImGui_ImplSdlGL3_Init(App->window->window);
	
	// Set camera initial pos
	App->camera->Look(float3(1.75f, 1.75f, 5.0f), float3(0.0f, 0.0f, 0.0f));

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	//BROFILER_CATEGORY("ModulePhysics3D::Generate_Heightmap", Profiler::Color::LightBlue);

	//Color c = App->camera->back

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	ImGui_ImplSdlGL3_NewFrame(App->window->window);

	return UPDATE_CONTINUE;
}

// Update: debug camera
update_status ModuleRenderer3D::Update(float dt)
{
	//Depth Test Enabling/Disabling
	if (isDepthTest && !isDepthTestOnce)
	{ 
		glEnable(GL_DEPTH_TEST);
		isDepthTestOnce = true;
		isDepthTestDOnce = false;
		console->LogConsole("DepthTest Enabled\n");
	}
	else if(!isDepthTest && !isDepthTestDOnce)
	{
		glDisable(GL_DEPTH_TEST);
		isDepthTestOnce = false;
		isDepthTestDOnce = true;
		console->LogConsole("DepthTest Disabled\n");
	}
	//Cull Face Enabling/Disabling
	if (isCullFace && !isCullFaceOnce)
	{
		glEnable(GL_CULL_FACE);
		isCullFaceOnce = true;
		isCullFaceDOnce = false;
		console->LogConsole("Cullface Enabled\n");
	}
	else if (!isCullFaceOnce && !isCullFaceDOnce)
	{
		glDisable(GL_CULL_FACE);
		isCullFaceOnce = false;
		isCullFaceDOnce = true;
		console->LogConsole("Cullface Disabled\n");
	}
	
	//Lightning Enabling/Disabling
	if (isGLLightning && !isGLLightningOnce)
	{
		glEnable(GL_LIGHTING);
		isGLLightningOnce = true;
		isGLLightningDOnce = false;
		console->LogConsole("Lightning Enabled\n");
	}
	else if (!isGLLightning && !isGLLightningDOnce)
	{
		glDisable(GL_LIGHTING);
		isGLLightningOnce = false;
		isGLLightningDOnce = true;
		console->LogConsole("Lightning Disabled\n");
	}
	
	//Color Material Enabling/Disabling
	if (isGLColorMaterial && !isGLColorMaterialOnce)
	{
		glEnable(GL_COLOR_MATERIAL);
		isGLColorMaterialOnce = true;
		isGLColorMaterialDOnce = false;
		console->LogConsole("GL Color Material Enabled\n");
	}
	else if (!isGLColorMaterial && !isGLColorMaterialDOnce)
	{
		glDisable(GL_COLOR_MATERIAL);
		isGLColorMaterialOnce = false;
		isGLColorMaterialDOnce = true;
		console->LogConsole("GL Color Material Disabled\n");
	}

	//GL Texture 2D Enabling/Disabling
	if (isGLTexture2D && !isGLTexture2DOnce)
	{
		glEnable(GL_TEXTURE_2D);
		isGLTexture2DOnce = true;
		isGLTexture2DDOnce = false;
		console->LogConsole("GL Texture 2D Enabled\n");
	}
	else if (!isGLTexture2D && !isGLTexture2DDOnce)
	{
		glDisable(GL_TEXTURE_2D);
		isGLTexture2DOnce = false;
		isGLTexture2DDOnce = true;
		console->LogConsole("GL Texture 2D Disabled\n");
	}

	return UPDATE_CONTINUE;

}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	//BROFILER_CATEGORY("ModulePhysics3D::Generate_Heightmap", Profiler::Color::MediumBlue);

	DrawGeometry();
	App->editor->Draw();
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	console->LogConsole("Destroying 3D Renderer\n");
	ImGui_ImplSdlGL3_Shutdown();
	SDL_GL_DeleteContext(context);
	return true;
}

void ModuleRenderer3D::Save(JSONNode* config) const
{
	config->PushBool("VSYNC", App->config_values.vsync);
}

void ModuleRenderer3D::Load(JSONNode* config)
{
	App->config_values.vsync = config->PullBool("VSYNC", false);
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/* OpenGL perspective projection matrix function calls:
		- ProjectionMatrix = OpenGLPerspProjLH(float n, float f, float h, float v);
		- ProjectionMatrix = OpenGLPerspProjRH(float n, float f, float h, float v);

	In OpenGL, the post-perspective unit cube ranges in [-1, 1] in all X, Y and Z directions.
	OpenGLPerspProjLH is the same as OpenGLPerspProjRH, except that the camera looks towards +Z in view space, instead of -Z.

	Although these functions change the projection matrix, it will eventually depend on the fustrum culling.
	Furthermore OnResize should more conveniently trigger an event the later updates the projection matrix.

	Therefore, for now trashy code will have to suffice.*/

	mat4x4 tmp = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);

	for (int row = 0; row < 4; row++)
		for (int col = 0; col < 4; col++)
			ProjectionMatrix[row][col] = tmp[(row * 4) + col];

	//ProjectionMatrix = (GLfloat*)ProjectionMatrix.v;
	GLfloat* tmp_GLFLoat = new GLfloat[16]();

	tmp_GLFLoat[0] = tmp.M[0];
	tmp_GLFLoat[1] = tmp.M[1];
	tmp_GLFLoat[2] = tmp.M[2];
	tmp_GLFLoat[3] = tmp.M[3];
	tmp_GLFLoat[4] = tmp.M[4];
	tmp_GLFLoat[5] = tmp.M[5];
	tmp_GLFLoat[6] = tmp.M[6];
	tmp_GLFLoat[7] = tmp.M[7];
	tmp_GLFLoat[8] = tmp.M[8];
	tmp_GLFLoat[9] = tmp.M[9];
	tmp_GLFLoat[10] = tmp.M[10];
	tmp_GLFLoat[11] = tmp.M[11];
	tmp_GLFLoat[12] = tmp.M[12];
	tmp_GLFLoat[13] = tmp.M[13];
	tmp_GLFLoat[14] = tmp.M[14];
	tmp_GLFLoat[15] = tmp.M[15];


	glLoadMatrixf(tmp_GLFLoat);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRenderer3D::DrawGeometry()
{
	// Render a cube
	//glBegin(GL_QUADS);
	
	if(isWireFramed)
		glBegin(GL_LINE_STRIP);
	else
		glBegin(GL_QUADS);
	
	// Top face
	glColor3f(0.0f, 1.0f, 0.0f);  // Green
	glVertex3f(1.0f, 1.0f, -1.0f);  // Top-right of top face
	glVertex3f(-1.0f, 1.0f, -1.0f);  // Top-left of top face
	glVertex3f(-1.0f, 1.0f, 1.0f);  // Bottom-left of top face
	glVertex3f(1.0f, 1.0f, 1.0f);  // Bottom-right of top face

								   // Bottom face
	glColor3f(1.0f, 0.5f, 0.0f); // Orange
	glVertex3f(1.0f, -1.0f, -1.0f); // Top-right of bottom face
	glVertex3f(-1.0f, -1.0f, -1.0f); // Top-left of bottom face
	glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom-left of bottom face
	glVertex3f(1.0f, -1.0f, 1.0f); // Bottom-right of bottom face

								   // Front face
	glColor3f(1.0f, 0.0f, 0.0f);  // Red
	glVertex3f(1.0f, 1.0f, 1.0f);  // Top-Right of front face
	glVertex3f(-1.0f, 1.0f, 1.0f);  // Top-left of front face
	glVertex3f(-1.0f, -1.0f, 1.0f);  // Bottom-left of front face
	glVertex3f(1.0f, -1.0f, 1.0f);  // Bottom-right of front face

									// Back face
	glColor3f(1.0f, 1.0f, 0.0f); // Yellow
	glVertex3f(1.0f, -1.0f, -1.0f); // Bottom-Left of back face
	glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom-Right of back face
	glVertex3f(-1.0f, 1.0f, -1.0f); // Top-Right of back face
	glVertex3f(1.0f, 1.0f, -1.0f); // Top-Left of back face

								   // Left face
	glColor3f(0.0f, 0.0f, 1.0f);  // Blue
	glVertex3f(-1.0f, 1.0f, 1.0f);  // Top-Right of left face
	glVertex3f(-1.0f, 1.0f, -1.0f);  // Top-Left of left face
	glVertex3f(-1.0f, -1.0f, -1.0f);  // Bottom-Left of left face
	glVertex3f(-1.0f, -1.0f, 1.0f);  // Bottom-Right of left face

									 // Right face
	glColor3f(1.0f, 0.0f, 1.0f);  // Violet
	glVertex3f(1.0f, 1.0f, 1.0f);  // Top-Right of left face
	glVertex3f(1.0f, 1.0f, -1.0f);  // Top-Left of left face
	glVertex3f(1.0f, -1.0f, -1.0f);  // Bottom-Left of left face
	glVertex3f(1.0f, -1.0f, 1.0f);  // Bottom-Right of left face
	
	glEnd();
}


bool ModuleRenderer3D::getDepthTest()
{
	return isDepthTest;
}

void ModuleRenderer3D::setDepthTest()
{
	isDepthTest = !isDepthTest;
}

bool ModuleRenderer3D::getCullFace()
{
	return isCullFace;
}

void ModuleRenderer3D::setCullFace()
{
	isCullFace = !isCullFace;
}

bool ModuleRenderer3D::getGLLightning()
{
	return isGLLightning;
}

void ModuleRenderer3D::setGLLightning()
{
	isGLLightning = !isGLLightning;
}

bool ModuleRenderer3D::getGLColorMaterial()
{
	return isGLColorMaterial;
}

void ModuleRenderer3D::setGLColorMaterial()
{
	isGLColorMaterial = !isGLColorMaterial;
}

bool ModuleRenderer3D::getGLTexture2D()
{
	return isGLTexture2D;
}

void ModuleRenderer3D::setGLTexture2D()
{
	isGLTexture2D = !isGLTexture2D;
}

bool ModuleRenderer3D::getWireFrame()
{
	return isWireFramed;
}

void ModuleRenderer3D::setWireFrame()
{
	isWireFramed = !isWireFramed;
}
