#ifndef __PRIMITIVE_H__
#define __PRIMITIVE_H__

#include "MathGeoLib\include\MathGeoLib.h"
#include "Color.h"

enum PrimitiveTypes
{
	Primitive_Point,
	Primitive_Line,
	Primitive_Plane,
	Primitive_Cube,
	Primitive_Sphere,
	Primitive_Cylinder
};

class Primitive
{
public:

	Primitive();

	virtual void	Render() const;
	virtual void	InnerRender() const;
	void			SetPos(const float x, const float y, const float z);
	void			SetPos(const float3 pos);
	void			SetRotation(const float angle, const float3 &u);
	void			SetRotation(const Quat rot);
	void			Scale(const float x, const float y, const float z);
	void			Scale(const float3 scale);
	PrimitiveTypes	GetType() const;

public:
	
	Color color;
	float4x4 transform;
	bool axis,wire;

protected:
	PrimitiveTypes type;
};

// ============================================
class PrimCube : public Primitive
{
public :
	PrimCube();
	PrimCube(float sizeX, float sizeY, float sizeZ);
	void InnerRender() const;
public:
	float3 size;
};

// ============================================
class PrimSphere : public Primitive
{
public:
	PrimSphere();
	PrimSphere(float radius);
	void InnerRender() const;
public:
	float radius;
};
//(btScalar*)
// ============================================
class PrimCylinder : public Primitive
{
public:
	PrimCylinder();
	PrimCylinder(float radius, float height);
	void InnerRender() const;
public:
	float radius;
	float height;
};

// ============================================
class PrimLine : public Primitive
{
public:
	PrimLine();
	PrimLine(float x, float y, float z);
	void InnerRender() const;
public:
	float3 origin;
	float3 destination;
};

// ============================================
class PrimPlane : public Primitive
{
public:
	PrimPlane();
	PrimPlane(float x, float y, float z, float d);
	void InnerRender() const;
public:
	float3 normal;
	float constant;
};

#endif