#ifndef __MODULECAMERA3D_H__
#define __MODULECAMERA3D_H__

#include "Module.h"
#include "MathGeoLib\include\MathGeoLib.h"

class SFrustum;

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(const char* name, bool start_enabled = true);
	~ModuleCamera3D();

	bool Init();
	update_status Update(float dt);
	bool CleanUp();
	
	void SetCameraPos(const vec pos);
	void SetAR(float ratio);
	void Focus(const vec pos);
	void FreeFocus();

	vec GetCameraPos() const;
	float* GetViewMatrix();
	float* GetProjMatrix();

	/*void Follow(PhysBody3D* body, float min, float max, float height);
	void UnFollow();
	void Look(const float3 &Position, const float3 &Reference, bool RotateAroundReference = false);
	void Move(const float3 &Movement);
	float* GetViewMatrix();*/

private:

	void HandleCameraInput(float dt);
	
public:

	float move_speed;
	float rotate_speed;
	float zoom_speed;
	bool proj_changed;

private:

	SFrustum* frust;
	vec focuspoint;
	bool is_focused;
};

#endif