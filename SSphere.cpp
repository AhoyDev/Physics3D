#include "SSphere.h"

#include "SSegment.h"
#include "SLine.h"
#include "SRay.h"
#include "SPlane.h"
#include "STriangle.h"
#include "SCircle.h"
#include "SPolygon.h"
#include "SFrustum.h"
#include "SCapsule.h"
#include "SPolyhedron.h"

SSphere::SSphere(float radius) : SPrimitive(SPHERE)
{
	shape = new Sphere(GetPos(), radius);
}

SSphere::SSphere(vec center, float radius) : SPrimitive(center, SPHERE)
{
	shape = new Sphere(GetPos(), radius);
}

bool SSphere::Intersects(const SSphere* other) const
{
	bool ret = false;

	if (other != nullptr)
		ret = shape->Intersects(*other->shape);

	return ret;
}
bool SSphere::Intersects(const SCapsule* other) const
{
	bool ret = false;

	if (other != nullptr)
		ret = shape->Intersects(*other->GetShape());

	return ret;
}