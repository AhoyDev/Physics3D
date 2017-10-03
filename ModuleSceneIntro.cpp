#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "GUI_Console.h"
#include "Event.h"
#include "ModuleRenderer3D.h"


ModuleSceneIntro::ModuleSceneIntro(const char* name, bool start_enabled) : Module(name, start_enabled),
graphics(NULL), ground(NULL)
{
	vertices = new float3[8];
	
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	console->LogConsole("Loading Intro assets\n");
	bool ret = true;
	
	//Init primitives
	initializeCube();

	//Loading assets
	//mesh = App->renderer3D->geometry_importer->LoadMesh("warrior.FBX", mesh);
	App->renderer3D->LoadMeshesOGL();


	//Cilinder
	CreateCilinder(1.f,1.f);
	CreateSphere(vec (0.f,0.f,0.f), 1.f, 12, 24);





	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	//console->LogConsole("Unloading Intro scene\n");
	
	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update(float dt)
{
	PrimPlane(0, 1, 0, 0).Render();





	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate(float dt)
{
	sphere->Render();
	
	App->renderer3D->DrawMesh(mesh);

	/*Primitives draw*/
	App->renderer3D->DrawCubeDirectMode();
	GLuint my_id = 1;
	App->renderer3D->DrawCubeGLDrawElements(my_id);
	//DrawCubeGLDrawArrays();
	//cilinder->DrawCilinder(cilinder->radius, cilinder->height,cilinder->R, cilinder->G,cilinder->B)



	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::ReceiveEvent(const Event& event)
{
	 // event.data.str contains const char* with filename of file dropped
	mesh = App->renderer3D->geometry_importer->LoadMesh(event.data.str, mesh);
}

void ModuleSceneIntro::DroppedFile(const char* file)
{
	LOG(file);
	mesh = App->renderer3D->geometry_importer->LoadMesh(file, mesh);
	App->renderer3D->LoadMeshesOGL();
}

void ModuleSceneIntro::initializeCube()
{

	vertices[0] = float3(1.0f, 1.0f, 1.0f);


	vertices[1] = float3(-1.0f, 1.0f, 1.0f);


	vertices[2] = float3(-1.0f, -1.0f, 1.0f);


	vertices[3] = float3(1.0f, -1.0f, 1.0f);

	vertices[4] = float3(1.0f, -1.0f, -1.0f);


	vertices[5] = float3(-1.0f, -1.0f, -1.0f);


	vertices[6] = float3(-1.0f, 1.0f, -1.0f);


	vertices[7] = float3(1.0f, 1.0f, -1.0f);




	my_id = 0;
	num_vertices = 8;


	glGenBuffers(1, (GLuint*) &(my_id));
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*num_vertices * 3, vertices, GL_STATIC_DRAW);




}

void ModuleSceneIntro::CreateCilinder(GLfloat radius, GLfloat height)
{
	//Cilinder
	cilinder = new SCilinder(radius, height);
}

void ModuleSceneIntro::CreateSphere(vec pos, float radius, unsigned int rings, unsigned int sectors)
{
	sphere = new SSphere(pos, radius, rings, sectors);
}

void ModuleSceneIntro::DrawCubeDirectMode()
{
	// Render a cube
	//glBegin(GL_QUADS);

	//if (isWireFramed)
	//	glBegin(GL_LINE_STRIP);
	//else
	//	
	glBegin(GL_QUADS);
	// Top face
	glColor3f(0.0f, 1.0f, 0.0f);  // Green
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);


	glColor3f(1.0f, 0.5f, 0.0f); // Orange
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);


	glColor3f(1.0f, 0.0f, 0.0f);  // Red
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);


	glColor3f(1.0f, 1.0f, 0.0f); // Yellow
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);


	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);


	glColor3f(1.0f, 0.0f, 1.0f);  // Violet
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	glEnd();
}


