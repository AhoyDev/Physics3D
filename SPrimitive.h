#ifndef __SPRIMITIVE_H__
#define __SPRIMITIVE_H__

#include "Color.h"
#include "MathGeoLib\include\MathGeoLib.h"
/*#include "MathGeoLib\include\Math\float3.h"
#include "MathGeoLib\include\Math/float4x4.h"
//#include "MathGeoLib\include\Geometry\AABB.h"
#include "MathGeoLib\include\Geometry\Capsule.h"
//#include "MathGeoLib\include\Geometry\Frustum.h"
//#include "MathGeoLib\include\Geometry\Line.h"
#include "MathGeoLib\include\Geometry\LineSegment.h"
//#include "MathGeoLib\include\Geometry\OBB.h"
//#include "MathGeoLib\include\Geometry\Plane.h"
//#include "MathGeoLib\include\Geometry\Polygon.h"
//#include "MathGeoLib\include\Geometry\Polyhedron.h"
//#include "MathGeoLib\include\Geometry\Ray.h"
#include "MathGeoLib\include\Geometry\Sphere.h"
//#include "MathGeoLib\include\Geometry\Triangle.h"*/

#include "MathGeoLib\include\Geometry\GeometryAll.h"

enum ShapeType
{
	DOT = 0,
	SEGMENT,
	LINE,
	RAY,
	PLANE,
	TRIANGLE,
	CIRCLE,
	POLYGON,
	CUBE,
	FRUSTUM,
	SPHERE,
	CYLINDER,
	CAPSULE,
	POLYHEDRON
};

class SSegment;
class SLine;
class SRay;
class SPlane;
class STriangle;
class SCircle;
class SPolygon;
class SCube;
class SFrustum;
class SSphere;
class SCylinder;
class SCapsule;
class SPolyhedron;
//class SCylinder ?

template<class s> class SPrimitive;
typedef SPrimitive<vec> SPoint;

template<class s>
class SPrimitive
{
public:

	float4x4	transform;
	//Color		color;

protected:

	s*			shape;
	ShapeType	type;

public:
	SPrimitive(ShapeType type = DOT) : type(type), transform(float4x4::identity)
	{
		SetPos(0.f, 0.f, 0.f); // set to origin
	}

	SPrimitive(vec pos, ShapeType type = DOT) : type(type), transform(float4x4::identity)
	{
		SetPos(pos);
	}

	virtual ~SPrimitive()
	{
		delete shape;
	}

	// RENDERS ============================================
	virtual void Render() const {}
	virtual void InnerRender() const {}

	// SETTERS & TRANSFORMS ===============================
	virtual void SetPos(const float x, const float y, const float z)
	{
		transform.v[3][0] = x;
		transform.v[3][1] = y;
		transform.v[3][2] = z;
	}

	virtual void SetPos(const float3 pos)
	{
		SetPos(pos.x, pos.y, pos.z);
	}

	virtual void SetRotation(const float angle, const float3 &u)
	{
		transform.SetRotatePart(u, angle);
	}

	virtual void SetRotation(const Quat rot)
	{
		transform.SetRotatePart(rot);
	}

	virtual void Scale(const float x, const float y, const float z)
	{
		float3x3 mat = transform.Scale(x, y, z).ToFloat3x3();
		transform.Set3x3Part(transform.Float3x3Part() * mat);
	}

	virtual void Scale(const float3 scale)
	{
		float3x3 mat = transform.Scale(scale).ToFloat3x3();
		transform.Set3x3Part(transform.Float3x3Part() * mat);
	}

	vec GetPos() const
	{
		vec pos;
		pos.x = transform.v[3][0];
		pos.y = transform.v[3][1];
		pos.z = transform.v[3][2];
		return pos;
	}

	// GETTERS ============================================
	ShapeType GetType() const
	{
		return type;
	}

	s* GetShape() const
	{
		return shape;
	}

	// INTERSECTS ==========================================
	virtual bool Intersects(const SPoint* other = nullptr) const { return false; }
	virtual bool Intersects(const SSegment* other = nullptr) const { return false; }
	virtual bool Intersects(const SLine* other = nullptr) const { return false; }
	virtual bool Intersects(const SRay* other = nullptr) const { return false; }
	virtual bool Intersects(const SPlane* other = nullptr) const { return false; }
	virtual bool Intersects(const STriangle* other = nullptr) const { return false; }
	virtual bool Intersects(const SCircle* other = nullptr) const { return false; }
	virtual bool Intersects(const SPolygon* other = nullptr) const { return false; }
	//virtual bool Intersects(const SCube* other = nullptr) const { return false; }
	virtual bool Intersects(const SFrustum* other = nullptr) const { return false; }
	virtual bool Intersects(const SSphere* other = nullptr) const { return false; }
	virtual bool Intersects(const SCapsule* other = nullptr) const { return false; }
	virtual bool Intersects(const SPolyhedron* other = nullptr) const { return false; }

};

#endif
