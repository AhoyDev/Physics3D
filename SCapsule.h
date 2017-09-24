#ifndef __SCAPSULE_H__
#define __SCAPSULE_H__

#include "SPrimitive.h"

class SCapsule : public SPrimitive<Capsule>
{
public:
	SCapsule(float radius = 1.f);
	SCapsule(const SSegment endPoints, float radius = 1.f);

	/*bool Intersects(const SPoint* other = nullptr) const;
	bool Intersects(const SSegment* other = nullptr) const;
	bool Intersects(const SLine* other = nullptr) const;
	bool Intersects(const SRay* other = nullptr) const;
	bool Intersects(const SPlane* other = nullptr) const;
	bool Intersects(const STriangle* other = nullptr) const;
	bool Intersects(const SCircle* other = nullptr) const;
	bool Intersects(const SPolygon* other = nullptr) const;
	//bool Intersects(const SCube* other = nullptr) const;
	bool Intersects(const SFrustum* other = nullptr) const;*/
	bool Intersects(const SSphere* other = nullptr) const;
	bool Intersects(const SCapsule* other = nullptr) const;
	//bool Intersects(const SPolyhedron* other = nullptr) const;

};

#endif