#ifndef __MODULESCENEINTRO_H__
#define __MODULESCENEINTRO_H__

#include "Module.h"
#include "SCilinder.h"
#include "SSphere.h"
#include "GeometryImporter.h"
#include "SCube.h"

class SDL_Texture;

#define BOUNCER_TIME 200

struct PhysBody3D;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(const char* name, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void ReceiveEvent(const Event& event);

	void DroppedFile(const char* file);

public:

	SDL_Texture* graphics;
	PhysBody3D* ground;

	


	//Methods for primitives
private:
	void initializeCube();
	void CreateCilinder(GLfloat radius, GLfloat height);
	void CreateSphere(vec pos, float radius, unsigned int rings, unsigned int sectors);

	//Temporary objects in the scene

	float3*  vertices;
	int		num_vertices;
	uint	my_id;

	//Cilinder
	SCilinder* cilinder;



	//Sphere
	SSphere* sphere;

	
	RMesh* mesh = nullptr;

	//cube
	SCube* cube_checkers;
	SCube* cube_arrays;


};

#endif