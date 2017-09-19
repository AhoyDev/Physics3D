#include "ModulePhysics3D.h"

#include "Application.h"
#include "ModuleInput.h"
#include "ModuleCamera3D.h"
#include "PhysBody3D.h"
#include "SDL\include\SDL.h"
#include "Bullet/include/btBulletDynamicsCommon.h"
#include "Bullet/include/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"

#ifdef _DEBUG
#pragma comment (lib, "Bullet/bin/BulletDynamics_Debug.lib")
#pragma comment (lib, "Bullet/bin/BulletCollision_Debug.lib")
#pragma comment (lib, "Bullet/bin/LinearMath_Debug.lib")
#else
#pragma comment (lib, "Bullet/bin/BulletDynamics_Debug.lib")
#pragma comment (lib, "Bullet/bin/BulletCollision_Debug.lib")
#pragma comment (lib, "Bullet/bin/LinearMath_Debug.lib")
#endif

ModulePhysics3D::ModulePhysics3D(bool start_enabled) : Module(start_enabled)
{
	debug = false;
}

// Destructor
ModulePhysics3D::~ModulePhysics3D()
{
	delete debug_draw;
	delete solver;
	delete broad_phase;
	delete dispatcher;
	delete collision_conf;
}

// Render not available yet----------------------------------
bool ModulePhysics3D::Init()
{
	LOG("Creating 3D Physics simulation");
	bool ret = true;

	collision_conf = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collision_conf);
	broad_phase = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver;
	debug_draw = new DebugDrawer();

	return ret;
}

// ---------------------------------------------------------
bool ModulePhysics3D::Start()
{
	LOG("Creating Physics environment");

	world = new btDiscreteDynamicsWorld(dispatcher, broad_phase, solver, collision_conf);
	world->setDebugDrawer(debug_draw);
	world->setGravity(GRAVITY);
	vehicle_raycaster = new btDefaultVehicleRaycaster(world);

	return true;
}

// ---------------------------------------------------------
PhysBody3D* ModulePhysics3D::AddBody(const PrimCube& cube, float mass)
{
	btCollisionShape* colShape = new btBoxShape(btVector3(cube.size.x*0.5f, cube.size.y*0.5f, cube.size.z*0.5f));

	shapes.push_back(colShape);

	btTransform startTransform;
	startTransform.setFromOpenGLMatrix((btScalar*)cube.transform.v);

	btVector3 localInertia(0, 0, 0);
	if (mass != 0.f)
		colShape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);
	PhysBody3D* pbody = new PhysBody3D(body);

	body->setUserPointer(pbody);
	world->addRigidBody(body);
	bodies.push_back(pbody);

	return pbody;
}

// ---------------------------------------------------------
PhysBody3D* ModulePhysics3D::AddBody(const PrimSphere& sphere, float mass)
{
	btCollisionShape* colShape = new btSphereShape(sphere.radius);
	shapes.push_back(colShape);

	btTransform startTransform;
	startTransform.setFromOpenGLMatrix((btScalar*)sphere.transform.v);

	btVector3 localInertia(0, 0, 0);
	if (mass != 0.f)
		colShape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);
	PhysBody3D* pbody = new PhysBody3D(body);

	body->setUserPointer(pbody);
	world->addRigidBody(body);
	bodies.push_back(pbody);

	return pbody;
}

// ---------------------------------------------------------
PhysBody3D* ModulePhysics3D::AddBody(const PrimCylinder& cylinder, float mass)
{
	btCollisionShape* colShape = new btCylinderShapeX(btVector3(cylinder.height*0.5f, cylinder.radius * 2, 0.0f));
	shapes.push_back(colShape);

	btTransform startTransform;
	startTransform.setFromOpenGLMatrix((btScalar*)cylinder.transform.v);

	btVector3 localInertia(0, 0, 0);
	if (mass != 0.f)
		colShape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);
	PhysBody3D* pbody = new PhysBody3D(body);

	body->setUserPointer(pbody);
	world->addRigidBody(body);
	bodies.push_back(pbody);

	return pbody;
}

// ---------------------------------------------------------
PhysBody3D* ModulePhysics3D::AddBody(const PrimPlane& plane)
{
	btCollisionShape* colShape = new btStaticPlaneShape(btVector3(plane.normal.x, plane.normal.y, plane.normal.z), plane.constant);
	shapes.push_back(colShape);

	btTransform startTransform;
	startTransform.setFromOpenGLMatrix((btScalar*)plane.transform.v);

	btVector3 localInertia(0, 0, 0);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0f, myMotionState, colShape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);
	PhysBody3D* pbody = new PhysBody3D(body);

	body->setUserPointer(pbody);
	world->addRigidBody(body);
	bodies.push_back(pbody);

	return pbody;
}

// ---------------------------------------------------------
PhysBody3D*	ModulePhysics3D::AddHeighField(const char* filename, int width, int length)
{
	unsigned char* heightfieldData = new unsigned char[width*length];
	{
		for (int i = 0; i<width*length; i++)
			heightfieldData[i] = 0;
	}

	FILE* heightfieldFile;
	fopen_s(&heightfieldFile, filename, "r");
	if (heightfieldFile)
	{
		int numBytes = fread(heightfieldData, 1, width*length, heightfieldFile);
		//btAssert(numBytes);
		if (!numBytes)
		{
			printf("couldn't read heightfield at %s\n", filename);
		}
		fclose(heightfieldFile);
	}

	//btScalar maxHeight = 20000.f;//exposes a bug
	btScalar maxHeight = 100;

	bool useFloatDatam = false;
	bool flipQuadEdges = false;

	int upIndex = 1;

	btHeightfieldTerrainShape* heightFieldShape = new btHeightfieldTerrainShape(width, length, heightfieldData, maxHeight, upIndex, useFloatDatam, flipQuadEdges);
	btVector3 mmin, mmax;
	heightFieldShape->getAabb(btTransform::getIdentity(), mmin, mmax);

	btCollisionShape* groundShape = heightFieldShape;

	heightFieldShape->setUseDiamondSubdivision(true);

	btVector3 localScaling(10, 1, 10);
	localScaling[upIndex] = 1.f;
	groundShape->setLocalScaling(localScaling);
	shapes.push_back(groundShape);

	//create ground object

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(0, 49.4, 0));

	btVector3 localInertia(0, 0, 0);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0f, myMotionState, groundShape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);
	PhysBody3D* pbody = new PhysBody3D(body);

	body->setUserPointer(pbody);
	world->addRigidBody(body);
	bodies.push_back(pbody);

	return pbody;
}



// ---------------------------------------------------------
void ModulePhysics3D::DeleteBody(PhysBody3D* pbody)
{
	/*
	if(pbody->body && pbody->body->getMotionState())
	delete pbody->body->getMotionState();

	world->removeCollisionObject(pbody->body);

	delete pbody->body;
	pbody->body = NULL;

	delete pbody->collision_shape;
	pbody->collision_shape = NULL;
	*/
	// TODO: remove from the array "bodies"
}

// ---------------------------------------------------------
update_status ModulePhysics3D::PreUpdate(float dt)
{
	// Step the physics world
	world->stepSimulation(dt, 15);

	// Detect collisions
	int numManifolds = world->getDispatcher()->getNumManifolds();
	for (int i = 0; i<numManifolds; i++)
	{
		btPersistentManifold* contactManifold = world->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject* obA = (btCollisionObject*)(contactManifold->getBody0());
		btCollisionObject* obB = (btCollisionObject*)(contactManifold->getBody1());

		int numContacts = contactManifold->getNumContacts();
		if (numContacts > 0)
		{
			PhysBody3D* pbodyA = (PhysBody3D*)obA->getUserPointer();
			PhysBody3D* pbodyB = (PhysBody3D*)obB->getUserPointer();

			if (pbodyA && pbodyB)
			{
				std::list<Module*>::iterator item = pbodyA->collision_listeners.begin();
				for (; item != pbodyA->collision_listeners.end(); item++)
				{
					(*item)->OnCollision(pbodyA, pbodyB);
				}
				
				item = pbodyA->collision_listeners.begin();
				for (; item != pbodyA->collision_listeners.end(); item++)
				{
					(*item)->OnCollision(pbodyB, pbodyA);
				}
			}
		}
	}

	return UPDATE_CONTINUE;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if (debug == true)
	{
		world->debugDrawWorld();

		// drop primitives on 1,2,3
		if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		{
			PrimSphere s(1);
			s.SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);
			AddBody(s);
		}

		if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		{
			PrimCube c(1, 1, 1);
			c.SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);
			AddBody(c);
		}

		if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
		{
			PrimCylinder c(0.5, 1);
			c.SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);
			AddBody(c);
		}
	}

	return UPDATE_CONTINUE;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModulePhysics3D::CleanUp()
{
	LOG("Destroying 3D Physics simulation");

	// Free all the bodies ---
	for (int i = world->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = world->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		world->removeCollisionObject(obj);
		delete obj;
	}

	// Free all collision shapes
	std::list<btCollisionShape*>::iterator s_item = shapes.begin();
	for (; s_item != shapes.end(); s_item++)
	{
		delete (*s_item);
		s_item = shapes.erase(s_item);
	}

	std::list<PhysBody3D*>::iterator b_item = bodies.begin();
	for (; b_item != bodies.end(); b_item++)
	{
		delete (*b_item);
		b_item = bodies.erase(b_item);
	}

	// Order matters !
	delete vehicle_raycaster;
	delete world;

	return true;
}

// =============================================
void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	line.origin.Set(from.getX(), from.getY(), from.getZ());
	line.destination.Set(to.getX(), to.getY(), to.getZ());
	line.color.Set(color.getX(), color.getY(), color.getZ());
	line.Render();
}

void DebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
	point.transform.Translate(PointOnB.getX(), PointOnB.getY(), PointOnB.getZ());
	point.color.Set(color.getX(), color.getY(), color.getZ());
	point.Render();
}

void DebugDrawer::reportErrorWarning(const char* warningString)
{
	LOG("Bullet warning: %s", warningString);
}

void DebugDrawer::draw3dText(const btVector3& location, const char* textString)
{
	LOG("Bullet draw text: %s", textString);
}

void DebugDrawer::setDebugMode(int debugMode)
{
	mode = (DebugDrawModes)debugMode;
}

int	 DebugDrawer::getDebugMode() const
{
	return mode;
}
