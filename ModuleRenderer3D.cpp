#include "ModuleRenderer3D.h"

#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleCamera3D.h"
#include "ModuleEditor.h"
#include "GUI_Console.h"
#include "JSONNode.h"
#include "Glew\include\glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "GeometryImporter.h"

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/libx86/32bits/glew32.lib") 

#include "Imgui\imgui.h"
#include "Imgui\imgui_impl_sdl_gl3.h"

#include "Brofiler/include/Brofiler.h"

// TEMPORAL
#include "glmath.h"

#define CHECKERS_HEIGHT 128 
#define CHECKERS_WIDTH 128

ModuleRenderer3D::ModuleRenderer3D(const char* name, bool start_enabled) : Module(name, start_enabled)
{
	
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
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

	geometry_importer = new GeometryImporter();

	

	GLubyte checkImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
	for (int i = 0; i < CHECKERS_HEIGHT; i++) {
		for (int j = 0; j < CHECKERS_WIDTH; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}

	

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &ImageName);
	glBindTexture(GL_TEXTURE_2D, ImageName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);



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


	//Editor Draws
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

void ModuleRenderer3D::DrawCubeDirectMode()
{
	// Render a cube
	
	



	glBindTexture(GL_TEXTURE_2D, ImageName);
	glEnable(GL_TEXTURE_2D);
	
	if(isWireFramed)
		glBegin(GL_LINE_STRIP);
	else
		glBegin(GL_TRIANGLES);
	
	
	
	// Top face
	//glColor3f(0.0f, 1.0f, 0.0f);  // Green
	

	
	//Top triangle 1
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);  //b
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);	//c
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);	//d*/


	// second triangle, top right half


	//Top triangle 2	   
	//glColor3f(1.0f, 0.5f, 0.0f); // Orange
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);	//b
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);	//d
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);		//a
	
	

	//Bot triangle 1							   
	//glColor3f(1.0f, 0.0f, 0.0f);  // Red
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);	//g
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);  //f
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);  //h
	


	//Bot triangle 2								
	//glColor3f(1.0f, 1.0f, 0.0f); // Yellow
	glVertex3f(1.0f, -1.0f, -1.0f); //h
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f); //f
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f); //e
	glTexCoord2f(1.0f, 0.0f);
	

								   
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);//b
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);  //c
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f); //g
	


	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);//a
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);  //d
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);  //h

	


	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);//a
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 1.0f); //e
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);  //h


	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);//b
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f); //g
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f); //f

	//

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);  //c
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f); //g
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);  //d

	
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f); //g
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);  //h
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);  //d

	


	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);//b
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f); //f
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f); //e


	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);//b
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f); //e
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);//a


	






	
	
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	

}

void ModuleRenderer3D::DrawCubeGLDrawElements(GLuint my_id)
{
	GLuint g_Indices[24] = {
		0, 1, 2, 3,
		7, 4, 5, 6,
		6, 5, 2, 1,
		7, 0, 3, 4,
		7, 6, 1, 0,
		3, 2, 5, 4,
	};

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER,my_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	// ... draw other buffers
	//glDrawArrays(GL_TRIANGLES, 0, num_vertices * 3);
	glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, &g_Indices[0]);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void ModuleRenderer3D::DrawCubeGLDrawArrays(GLuint my_id, int num_vertices)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	// ... draw other buffers
	glDrawArrays(GL_TRIANGLES, 0, num_vertices * 3);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void ModuleRenderer3D::LoadMeshesOGL()
{
	LOG("Loading Intro assets");


	for (int i = 0; i < geometry_importer->meshes.size(); i++)
	{

		glGenBuffers(1, (GLuint*) &(geometry_importer->meshes[i]->id_vertices));

		glBindBuffer(GL_ARRAY_BUFFER, geometry_importer->meshes[i]->id_vertices);

		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*geometry_importer->meshes[i]->numVertex * 3, geometry_importer->meshes[i]->vertices, GL_STATIC_DRAW);

		glGenBuffers(1, (GLuint*) &(geometry_importer->meshes[i]->id_indices));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry_importer->meshes[i]->id_indices);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*geometry_importer->meshes[i]->numFaces, geometry_importer->meshes[i]->indices, GL_STATIC_DRAW);


		glEnableClientState(GL_NORMAL_ARRAY);


		glGenBuffers(1, (GLuint*) &(geometry_importer->meshes[i]->id_normals));
		glBindBuffer(GL_ARRAY_BUFFER, geometry_importer->meshes[i]->id_normals);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) *geometry_importer->meshes[i]->numVertex * 3, geometry_importer->meshes[i]->normals, GL_STATIC_DRAW);
		glNormalPointer(GL_FLOAT, 0, NULL);

	}


}

void ModuleRenderer3D::initializeTexture()
{

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


bool ModuleRenderer3D::DrawMesh(RMesh* mesh)
{
	bool ret = true;

	if(geometry_importer->meshes.size() > 0)
	{
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);



			glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertices);
			glVertexPointer(3, GL_FLOAT, 0, NULL);

			glBindBuffer(GL_ARRAY_BUFFER, mesh->id_normals);
			glNormalPointer(GL_FLOAT, 0, NULL);

			glBindBuffer(GL_ARRAY_BUFFER, mesh->id_indices);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices);
			glDrawElements(GL_TRIANGLES, mesh->numFaces, GL_UNSIGNED_INT, NULL);



			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
	}
	return ret;
}