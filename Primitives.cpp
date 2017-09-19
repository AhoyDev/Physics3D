#include "Primitives.h"
#include "R_Sphere.h"

R_Capsule::R_Capsule(LineSegment _line_segment, float _radius)
{
	capsule = new Capsule(_line_segment,_radius);
}

bool R_Capsule::Intersects(R_Sphere _capsule)
{
	bool ret = false;

	ret = capsule->Intersects(*_capsule.sphere);

	return ret;
}
