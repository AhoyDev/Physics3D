#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include "MathGeoLib\include\MathGeoLibFwd.h"
#include "MathGeoLib\include\Math\float3.h"

class Primitive;
class PhysBody3D;

enum COLLIDER_TYPE
{
	C_CUBE = 0,
	C_SPHERE
};

class Collider
{
public:
	Collider(COLLIDER_TYPE type = COLLIDER_TYPE(0), bool render = true);
	Collider(vec initial_pos, COLLIDER_TYPE type = COLLIDER_TYPE(0), bool render = true);
	~Collider();

	bool Intersect(Collider* other) const;
	void Update();
	void SetShape(COLLIDER_TYPE type);

public:

	vec pos; // IMPORTANT not enough for intersect -> need full transform
	bool render;

private:

	COLLIDER_TYPE type;
	Primitive* shape;
	PhysBody3D* body = nullptr;
};

#endif