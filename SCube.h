#ifndef __SCUBE_H__
#define __SCUBE_H__

#include "SPrimitive.h"

// Cube undefined in MathGeoLib
class Cube;

class SCube : public SPrimitive<Cube>
{
public:
	SCube() {}
	SCube(float sizeX, float sizeY, float sizeZ) {}

	/*bool Intersects(const SPoint* other = nullptr) const;
	bool Intersects(const SSegment* other = nullptr) const;
	bool Intersects(const SLine* other = nullptr) const;
	bool Intersects(const SRay* other = nullptr) const;
	bool Intersects(const SPlane* other = nullptr) const;
	bool Intersects(const STriangle* other = nullptr) const;
	bool Intersects(const SCircle* other = nullptr) const;
	bool Intersects(const SPolygon* other = nullptr) const;
	//bool Intersects(const SCube* other = nullptr) const;
	bool Intersects(const SFrustum* other = nullptr) const;
	bool Intersects(const SSphere* other = nullptr) const;
	bool Intersects(const SCapsule* other = nullptr) const;
	bool Intersects(const SPolyhedron* other = nullptr) const;*/

};

#endif

