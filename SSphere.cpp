#include "SSphere.h"

#include "SSegment.h"
#include "SLine.h"
#include "SRay.h"
#include "SPlane.h"
#include "STriangle.h"
#include "SCircle.h"
#include "SPolygon.h"
#include "SFrustum.h"
#include "SCapsule.h"
#include "SPolyhedron.h"

#include "MathGeoLib\include\Math\MathConstants.h"
#include "MathGeoLib\include\Math\MathFunc.h"

SSphere::SSphere(float radius, unsigned int rings, unsigned int sectors) : SPrimitive(SPHERE)
{
	SSphere(vec(0.f), radius, rings, sectors);
}

SSphere::SSphere(vec center, float radius, unsigned int rings, unsigned int sectors) : SPrimitive(center, SPHERE)
{
	// Define Shape
	shape = new Sphere(GetPos(), radius);
	
	int r, s;
	float x, y, z;
	float const R = 1.f / (float)(rings - 1);
	float const S = 1.f / (float)(sectors - 1);

	// Adapt containers to fit data
	vertices.resize(rings * sectors * 3);
	normals.resize(rings * sectors * 3);
	texcoords.resize(rings * sectors * 2);
	indices.resize(rings * sectors * 4);

	// Define vectors for gl
	std::vector<GLfloat>::iterator v = vertices.begin();
	std::vector<GLfloat>::iterator n = normals.begin();
	std::vector<GLfloat>::iterator t = texcoords.begin();
	for (r = 0; r < rings; r++)
	{
		for (s = 0; s < sectors; s++)
		{
			x = Cos(2 * pi * s * S) * sin(pi * r * R);
			y = Sin(-pi2 + pi * r * R);
			z = Sin(2 * pi * s * S) * sin(pi * r * R);

			// vertices
			*v++ = x * radius;
			*v++ = y * radius;
			*v++ = z * radius;

			// normals
			*n++ = x;
			*n++ = y;
			*n++ = z;

			// Texture coords
			*t++ = s*S;
			*t++ = r*R;
		}
	}
	
	std::vector<GLushort>::iterator i = indices.begin();
	for (r = 0; r < rings - 1; r++)
	{
		for (s = 0; s < sectors - 1; s++)
		{
			*i++ = r * sectors + s;
			*i++ = r * sectors + (s + 1);
			*i++ = (r + 1) * sectors + (s + 1);
			*i++ = (r + 1) * sectors + s;
		}
	}
}

void SSphere::Render() const
{
	//GLfloat x, GLfloat y, GLfloat z
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	
	// Translate
	glTranslatef(shape->pos.x, shape->pos.y, shape->pos.z);
	
	// Scale
	vec scale = transform.ExtractScale();
	glScalef(scale.x, scale.y, scale.z);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
	glNormalPointer(GL_FLOAT, 0, &normals[0]);
	glTexCoordPointer(2, GL_FLOAT, 0, &texcoords[0]);
	glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, &indices[0]);
	glPopMatrix();
}


bool SSphere::Intersects(const SSphere* other) const
{
	bool ret = false;

	if (other != nullptr)
		ret = shape->Intersects(*other->shape);

	return ret;
}
bool SSphere::Intersects(const SCapsule* other) const
{
	bool ret = false;

	if (other != nullptr)
		ret = shape->Intersects(*other->GetShape());

	return ret;
}
