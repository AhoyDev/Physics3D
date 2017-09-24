#include "SSegment.h"

#include "SLine.h"
#include "SRay.h"
#include "SPlane.h"
#include "STriangle.h"
#include "SCircle.h"
#include "SPolygon.h"
#include "SFrustum.h"
#include "SSphere.h"
#include "SCapsule.h"
#include "SPolyhedron.h"

SSegment::SSegment() : SPrimitive(SEGMENT)
{
	vec end = float3(0.f, 1.f, 0.f);
	shape = new LineSegment(GetPos(), end);
}

SSegment::SSegment(vec begin, vec end) : SPrimitive(((begin + end) / 2), SEGMENT)
{
	shape = new LineSegment(begin, end);
}