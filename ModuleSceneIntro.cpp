#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "GUI_Console.h"
#include "Event.h"
#include "ModuleRenderer3D.h"
#include "SCube.h"


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
	
	App->renderer3D->LoadMeshesOGL();


	//Cilinder
	CreateCilinder(1.f,1.f);
	CreateSphere(vec (0.f,0.f,0.f), 1.f, 12, 24);

	//Cube
	cube_checkers = new SCube(vec(3.f,3.f,3.f));
	//cube_arrays = new SCube(vec(1.f,1.f,1.f));

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{

	
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
	

	cube_checkers->InnerRender_Direct(vec(4.f,0.f,0.f));

	GLuint my_id = 1;
	//cube_arrays->InnerRender_Arrays(my_id,num_vertices);
	
	cilinder->DrawCilinder(cilinder->radius, cilinder->height, cilinder->R, cilinder->G, cilinder->B);



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

