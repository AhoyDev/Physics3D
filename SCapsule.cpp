#include "SCapsule.h"

#include "SSegment.h"
#include "SLine.h"
#include "SRay.h"
#include "SPlane.h"
#include "STriangle.h"
#include "SCircle.h"
#include "SPolygon.h"
#include "SFrustum.h"
#include "SSphere.h"
#include "SPolyhedron.h"

SCapsule::SCapsule(float radius) : SPrimitive(CAPSULE)
{
	vec top = float3(0.f, 1.f, 0.f);
	shape = new Capsule(GetPos(), top, radius);
}

SCapsule::SCapsule(const SSegment endPoints, float radius) : SPrimitive(endPoints.GetPos(), CAPSULE)
{
	shape = new Capsule((*endPoints.GetShape()), radius);
}

bool SCapsule::Intersects(const SSphere* other) const
{
	bool ret = false;

	if (other != nullptr)
		ret = shape->Intersects(*other->GetShape());

	return ret;
}
bool SCapsule::Intersects(const SCapsule* other) const
{
	bool ret = false;

	if (other != nullptr)
		ret = shape->Intersects(*other->GetShape());

	return ret;
}
