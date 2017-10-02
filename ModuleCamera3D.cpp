#include "ModuleCamera3D.h"

#include "Application.h"
#include "ModuleInput.h"
#include "SDL\include\SDL.h"
#include "PhysBody3D.h"
#include "GUI_Console.h"

// TEMPORAL
#include "glmath.h"


ModuleCamera3D::ModuleCamera3D(const char* name, bool start_enabled) : Module(name, start_enabled)
{
	CalculateViewMatrix();

	X = float3(1.0f, 0.0f, 0.0f);
	Y = float3(0.0f, 1.0f, 0.0f);
	Z = float3(0.0f, 0.0f, 1.0f);

	Position = float3(0.0f, 0.0f, 5.0f);
	Reference = float3(0.0f, 0.0f, 0.0f);

	following = NULL;
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	console->LogConsole("Setting up the camera\n");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	//LOG("Cleaning camera\n");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	// Follow code
	if(following != NULL)
	{
		float4x4 m;
		following->GetTransform((float*)m.v);

		Look(Position, m.Row3(3), true);

		// Correct height
		Position.y = (15.0*Position.y + Position.y + following_height) / 16.0;

		// Correct distance
		float3 cam_to_target = m.Row3(3) - Position;
		float dist = Length(cam_to_target);
		float correctionFactor = 0.f;
		if(dist < min_following_dist)
		{
			correctionFactor = 0.15*(min_following_dist - dist) / dist;
		}
		if(dist > max_following_dist)
		{
			correctionFactor = 0.15*(max_following_dist - dist) / dist;
		}
		Position -= correctionFactor * cam_to_target;
	}

	// Implement a debug camera with keys and mouse

	// OnKeys WASD keys -----------------------------------
	float Speed = 5.0f;

	if(App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) Speed *= 2.0f;
	if(App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT) Speed *= 0.5f;

	float Distance = Speed * dt;

	float3 Up(0.0f, 1.0f, 0.0f);
	float3 Right = X;
	float3 Forward = Cross(Up, Right);

	Up *= Distance;
	Right *= Distance;
	Forward *= Distance;

	float3 Movement(0,0,0);

	if(App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) Movement += Forward;
	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) Movement -= Forward;
	if(App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) Movement -= Right;
	if(App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) Movement += Right;
	if(App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) Movement += Up;
	if(App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) Movement -= Up;

	Position += Movement;
	Reference += Movement;
	
	// Mouse motion ----------------

	if(App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{

		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.25f;

		Position -= Reference;

		//---------------------------------------------------------------------------
		//
		// TRASH CODE
		//
		vec3 tmpX = vec3(X.x, X.y, X.z);
		vec3 tmpY = vec3(Y.x, Y.y, Y.z);
		vec3 tmpZ = vec3(Z.x, Z.y, Z.z);

		if(dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			//mat4x4 rotate(float angle, const vec3 &u)
			tmpX = rotate(tmpX, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			tmpY = rotate(tmpY, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			tmpZ = rotate(tmpZ, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		}

		if(dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			tmpY = rotate(tmpY, DeltaY, tmpX);
			tmpZ = rotate(tmpZ, DeltaY, tmpX);

			if(Y.y < 0.0f)
			{
				tmpZ = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				tmpY = cross(tmpZ, tmpX);
			}
		}

		X = float3(tmpX.x, tmpX.y, tmpX.z);
		Y = float3(tmpY.x, tmpY.y, tmpY.z);
		Z = float3(tmpZ.x, tmpZ.y, tmpZ.z);

		//----------------------------------------------------------------------------

		Position = Reference + Z * Length(Position);
	}

	// Mouse wheel -----------------------

	float zDelta = (float) App->input->GetMouseWheelMotion();

	Position -= Reference;

	if(zDelta < 0 && Length(Position) < 500.0f)
	{
		Position += Position * 0.1f;
	}

	if(zDelta > 0 && Length(Position) > 0.05f)
	{
		Position -= Position * 0.1f;
	}

	Position += Reference;

	// Recalculate matrix -------------
	CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const float3 &Position, const float3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = (Position - Reference).Normalized();
	X = (Cross(float3(0.0f, 1.0f, 0.0f), Z)).Normalized();
	Y = Cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::Move(const float3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return (float*)ViewMatrix.v;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = float4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -Dot(X, Position), -Dot(Y, Position), -Dot(Z, Position), 1.0f);
	ViewMatrixInverse = ViewMatrix.Inverted();
}

// -----------------------------------------------------------------
void ModuleCamera3D::Follow(PhysBody3D* body, float min, float max, float height)
{
	min_following_dist = min;
	max_following_dist = max;
	following_height = height;
	following = body;
}

// -----------------------------------------------------------------
void ModuleCamera3D::UnFollow()
{
	following = NULL;
}