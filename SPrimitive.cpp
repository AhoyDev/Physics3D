#include "SPrimitive.h"

// SEGMENT ============================================
SSegment::SSegment() : SPrimitive(SEGMENT)
{
	vec end = float3(0.f, 1.f, 0.f);
	shape = new LineSegment(GetPos(), end);
}

SSegment::SSegment(vec begin, vec end) : SPrimitive(((begin + end) / 2), SEGMENT)
{
	shape = new LineSegment(begin, end);
}


// SPHERE ============================================
SSphere::SSphere(float radius) : SPrimitive(SPHERE)
{
	shape = new Sphere(GetPos(), radius);
}

SSphere::SSphere(vec center, float radius) : SPrimitive(center, SPHERE)
{
	shape = new Sphere(GetPos(), radius);
}

bool SSphere::Intersects(const Sphere* other) const
{
	bool ret = false;

	if (other != nullptr)
		ret = shape->Intersects(*other);

	return ret;
}
bool SSphere::Intersects(const Capsule* other) const
{
	bool ret = false;

	if (other != nullptr)
		ret = shape->Intersects(*other);

	return ret;
}

// CAPSULE ============================================
SCapsule::SCapsule(float radius) : SPrimitive(CAPSULE)
{
	vec top = float3(0.f, 1.f, 0.f);
	shape = new Capsule(GetPos(), top, radius);
}

SCapsule::SCapsule(const SSegment endPoints, float radius) : SPrimitive(endPoints.GetPos(), CAPSULE)
{
	shape = new Capsule((*endPoints.GetShape()), radius);
}

bool SCapsule::Intersects(const Sphere* other) const
{
	bool ret = false;

	if (other != nullptr)
		ret = shape->Intersects(*other);

	return ret;
}
bool SCapsule::Intersects(const Capsule* other) const
{
	bool ret = false;

	if (other != nullptr)
		ret = shape->Intersects(*other);

	return ret;
}
