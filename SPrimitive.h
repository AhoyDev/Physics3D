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
	FUSTRUM,
	SPHERE,
	CYLINDER,
	CAPSULE,
	POLYHEDRON
};

class SSegment;
class SSphere;
class SCapsule;

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
	{}

	SPrimitive(vec pos, ShapeType type = DOT) : type(type), transform(float4x4::identity)
	{
		SetPos(pos);
	}

	virtual ~SPrimitive()
	{
		delete shape;
	}

	// RENDERS ============================================
	//virtual void	Render() const;
	//virtual void	InnerRender() const;

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
	virtual bool Intersects(const Sphere* other = nullptr) const
	{
		bool ret = false;

		if (other != nullptr)
			other->Contains(GetPos());

		return ret;
	}
	virtual bool Intersects(const Capsule* other = nullptr) const
	{
		bool ret = false;

		if (other != nullptr)
			other->Contains(GetPos());

		return ret;
	}
};

class SSegment : public SPrimitive<LineSegment>
{
public:
	SSegment();
	SSegment(vec begin, vec end);
};

/*class SLine : public Shape
{
public:
	SLine();
};

class SRay : public Shape
{
public:
	SRay();
};

class SPlane : public Shape
{
public:
	SPlane();
};*/

/*class STriangle : public Shape
{
public:
	STriangle();
};

class SCircle : public Shape
{
public:
	SCircle();
	SCircle(float radius);
	void InnerRender() const;
public:
	float radius;
};

class SPolygon : public Shape
{
public:
	SPolygon();
};

class SCube : public Shape
{
public:
	SCube();
	SCube(float sizeX, float sizeY, float sizeZ);
	void InnerRender() const;
public:
	float radius;
};

class SFustrum : public Shape
{
public:
	SFustrum();
};*/

class SSphere : public SPrimitive<Sphere>
{
public:
	SSphere(float radius = 1.f);
	SSphere(vec center, float radius = 1.f);

	bool Intersects(const Sphere* other = nullptr) const;
	bool Intersects(const Capsule* other = nullptr) const;
};

/*class SCylinder : public Shape
{
public:
	SCylinder();
};*/

class SCapsule : public SPrimitive<Capsule>
{
public:
	SCapsule(float radius = 1.f);
	SCapsule(const SSegment endPoints, float radius = 1.f);

	bool Intersects(const Sphere* other = nullptr) const;
	bool Intersects(const Capsule* other = nullptr) const;
};

/*class SPolyhedron : public Shape
{
public:
	SPolyhedron();
};*/


typedef SPrimitive<vec> SPoint;

SPoint::SPrimitive(vec pos, ShapeType type) : type(type), transform(float4x4::identity)
{
	SetPos(pos);
	shape = new vec(pos);
}
/*
bool Contains(const vec &point) const;
int Intersects(const LineSegment &lineSegment, vec *intersectionPoint = 0, vec *intersectionNormal = 0, float *d = 0, float *d2 = 0) const;
int Intersects(const Line &line, vec *intersectionPoint = 0, vec *intersectionNormal = 0, float *d = 0, float *d2 = 0) const;
int Intersects(const Ray &ray, vec *intersectionPoint = 0, vec *intersectionNormal = 0, float *d = 0, float *d2 = 0) const;
bool Intersects(const Plane &plane) const;
bool Intersects(const AABB &aabb, vec *closestPointOnAABB = 0) const;
bool Intersects(const OBB &obb, vec *closestPointOnOBB = 0) const;
bool Intersects(const Triangle &triangle, vec *closestPointOnTriangle = 0) const;
bool Intersects(const Capsule &capsule) const;
bool Intersects(const Polygon &polygon) const;
bool Intersects(const Frustum &frustum) const;
bool Intersects(const Polyhedron &polyhedron) const;
bool Intersects(const Sphere &sphere) const;*/

#endif
