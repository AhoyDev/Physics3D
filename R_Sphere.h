#ifndef __RSPHERE_H__
#define __RSPHERE_H__

#include "MathGeoLib\include\MathGeoLib.h"

class R_Capsule;

class R_Sphere
{
public:
	R_Sphere(vec, float);

	//Make a R_Primitive and derive all the other primitives to make it for a general primitive
	//Now only supports Capsules for the sake of speed
	bool Intersects(R_Capsule);


	Sphere* sphere;
};


bool Intersects(R_Sphere);

#endif