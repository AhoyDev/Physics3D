#ifndef __SSPHERE_H__
#define __SSPHERE_H__

#include "SPrimitive.h"

#include "Glew\include\glew.h"
#include <GL/gl.h>
#include <GL/glu.h>

class SSphere : public SPrimitive<Sphere>
{
public:
	SSphere(float radius = 1.f, unsigned int rings = 12, unsigned int sectors = 24);
	SSphere(vec center, float radius = 0.5f, unsigned int rings = 12, unsigned int sectors = 24);

	void Render() const;

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

private:

	std::vector<GLfloat> vertices;
	std::vector<GLfloat> normals;
	std::vector<GLfloat> texcoords;
	std::vector<GLushort> indices;
};

/* Sphere intersects:
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