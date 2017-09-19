#include "R_Sphere.h"
#include "Primitives.h"


R_Sphere::R_Sphere(math::vec _vec, float _radius)
{
	sphere = new Sphere(_vec, _radius);
}

bool R_Sphere::Intersects(R_Capsule _capsule)
{
	bool ret = false;

	ret = sphere->Intersects(*_capsule.capsule);

	return ret;
}