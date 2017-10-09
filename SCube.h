#ifndef __SCUBE_H__
#define __SCUBE_H__

#include "SPrimitive.h"
#include "Glew\include\glew.h"

// Cube undefined in MathGeoLib
class Cube;

class SCube : public SPrimitive<Cube>
{
public:
	SCube();
	SCube(vec size);

	void InnerRender_Direct(vec pos) const;
	void InnerRender_Arrays(GLuint my_id, int num_vertices);




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
	//To texture/gl methods
	GLuint* checkImage;
	GLuint ImageName;


};

class Cube
{
public:
	Cube();
	Cube(vec size);
	~Cube();

public:

	vec size;
	


};

#endif

