#ifndef __PRIMITIVES_H__
#define __PRIMITIVES_H__

#include "MathGeoLib/include/Geometry/Sphere.h"
#include "MathGeoLib/include/Geometry/Capsule.h"

#include "R_Sphere.h"

class R_Primitive
{
public:
	R_Primitive(){}
};


class R_Capsule
{
public:
	R_Capsule(LineSegment, float);

	bool Intersects(R_Sphere);
	Capsule* capsule;

};
#endif


