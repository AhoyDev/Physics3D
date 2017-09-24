#include "SPrimitive.h"

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

bool SPoint::Intersects(const SPoint* other) const
{
	bool ret = false;

	if (other != nullptr)
		ret = other->GetPos().Equals(GetPos());

	return ret;
}

bool SPoint::Intersects(const SSegment* other) const
{
	bool ret = false;

	if (other != nullptr)
		ret = other->GetShape()->Contains(GetPos());

	return ret;
}

bool SPoint::Intersects(const SLine* other) const
{
	bool ret = false;

	if (other != nullptr)
		ret = other->GetShape()->Contains(GetPos());

	return ret;
}

bool SPoint::Intersects(const SRay* other) const
{
	bool ret = false;

	if (other != nullptr)
		ret = other->GetShape()->Contains(GetPos());

	return ret;
}

bool SPoint::Intersects(const SPlane* other) const
{
	bool ret = false;

	if (other != nullptr)
		ret = other->GetShape()->Contains(GetPos());

	return ret;
}

bool SPoint::Intersects(const STriangle* other) const
{
	bool ret = false;

	if (other != nullptr)
		ret = other->GetShape()->Contains(GetPos());

	return ret;
}

bool SPoint::Intersects(const SCircle* other) const
{
	bool ret = false;

	if (other != nullptr)
		ret = other->GetShape()->DistanceToDisc(GetPos()) == 0;

	return ret;
}

bool SPoint::Intersects(const SPolygon* other) const
{
	bool ret = false;

	if (other != nullptr)
		ret = other->GetShape()->Contains(GetPos());

	return ret;
}

bool SPoint::Intersects(const SFrustum* other) const
{
	bool ret = false;

	if (other != nullptr)
		ret = other->GetShape()->Contains(GetPos());

	return ret;
}

bool SPoint::Intersects(const SSphere* other) const
{
	bool ret = false;

	if (other != nullptr)
		ret = other->GetShape()->Contains(GetPos());

	return ret;
}

bool SPoint::Intersects(const SCapsule* other) const
{
	bool ret = false;

	if (other != nullptr)
		ret = other->GetShape()->Contains(GetPos());

	return ret;
}

bool SPoint::Intersects(const SPolyhedron* other) const
{
	bool ret = false;

	if (other != nullptr)
		ret = other->GetShape()->Contains(GetPos());

	return ret;
}