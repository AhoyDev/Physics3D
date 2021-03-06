#include "SCube.h"

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

#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"
#include "Application.h"
#include "ModuleRenderer3D.h"


#define CHECKERS_HEIGHT 128 
#define CHECKERS_WIDTH 128

SCube::SCube() : SPrimitive(CUBE)
{
	shape = new Cube();


}

SCube::SCube(vec size) : SPrimitive(CUBE)
{
	shape = new Cube(size);

	GLubyte checkImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
	for (int i = 0; i < CHECKERS_HEIGHT; i++) {
		for (int j = 0; j < CHECKERS_WIDTH; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &ImageName);
	glBindTexture(GL_TEXTURE_2D, ImageName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);

	vertices = new float3[8];

	vertices[0] = float3(1.0f, 1.0f, 1.0f);


	vertices[1] = float3(-1.0f, 1.0f, 1.0f);


	vertices[2] = float3(-1.0f, -1.0f, 1.0f);


	vertices[3] = float3(1.0f, -1.0f, 1.0f);

	vertices[4] = float3(1.0f, -1.0f, -1.0f);


	vertices[5] = float3(-1.0f, -1.0f, -1.0f);


	vertices[6] = float3(-1.0f, 1.0f, -1.0f);


	vertices[7] = float3(1.0f, 1.0f, -1.0f);


	my_id = 0;
	num_vertices = 8;


	glGenBuffers(1, (GLuint*) &(my_id));
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*num_vertices * 3, vertices, GL_STATIC_DRAW);


}

void SCube::InnerRender_Direct(vec position) const
{
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, ImageName);
	glEnable(GL_TEXTURE_2D);
	glTranslatef(position.x,position.y ,position.z);
	if (App->renderer3D->isWireFramed)
		glBegin(GL_LINE_STRIP);
	else
		glBegin(GL_TRIANGLES);

	

	//Top triangle 1
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f*shape->size.x, 1.0f*shape->size.y, 1.0f*shape->size.z);  //b
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f*shape->size.x, 1.0f*shape->size.y, -1.0f*shape->size.z);	//c
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f*shape->size.x, 1.0f*shape->size.y, -1.0f*shape->size.z);	//d*/


								

									
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f*shape->size.x, 1.0f*shape->size.y, 1.0f*shape->size.z);	//b
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f*shape->size.x, 1.0f*shape->size.y, -1.0f*shape->size.z);	//d
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f*shape->size.x, 1.0f*shape->size.y, 1.0f*shape->size.z);		//a



	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f*shape->size.x, -1.0f*shape->size.y, -1.0f*shape->size.z);	//g
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f*shape->size.x, -1.0f*shape->size.y, 1.0f*shape->size.z);  //f
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f*shape->size.x, -1.0f*shape->size.y, -1.0f*shape->size.z);  //h



	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1.0f*shape->size.x, -1.0f*shape->size.y, -1.0f*shape->size.z); //h
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f*shape->size.x, -1.0f*shape->size.y, 1.0f*shape->size.z); //f
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f*shape->size.x, -1.0f*shape->size.y, 1.0f*shape->size.z); //e
	



	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-1.0f*shape->size.x, 1.0f*shape->size.y, 1.0f*shape->size.z);//b
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f*shape->size.x, 1.0f*shape->size.y, -1.0f*shape->size.z);  //c
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f*shape->size.x, -1.0f*shape->size.y, -1.0f*shape->size.z); //g



	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1.0f*shape->size.x, 1.0f*shape->size.y, 1.0f*shape->size.z);//a
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f*shape->size.x, 1.0f*shape->size.y, -1.0f*shape->size.z);  //d
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f*shape->size.x, -1.0f*shape->size.y, -1.0f*shape->size.z);  //h




	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1.0f*shape->size.x, 1.0f*shape->size.y, 1.0f*shape->size.z);//a
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f*shape->size.x, -1.0f*shape->size.y, 1.0f*shape->size.z); //e
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f*shape->size.x, -1.0f*shape->size.y, -1.0f*shape->size.z);  //h


	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-1.0f*shape->size.x, 1.0f*shape->size.y, 1.0f*shape->size.z);//b
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f*shape->size.x, -1.0f*shape->size.y, -1.0f*shape->size.z); //g
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f*shape->size.x, -1.0f*shape->size.y, 1.0f*shape->size.z); //f

									//

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f*shape->size.x, 1.0f*shape->size.y, -1.0f*shape->size.z);  //c
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f*shape->size.x, -1.0f*shape->size.y, -1.0f*shape->size.z); //g
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f*shape->size.x, 1.0f*shape->size.y, -1.0f*shape->size.z);  //d


	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f*shape->size.x, -1.0f*shape->size.y, -1.0f*shape->size.z); //g
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f*shape->size.x, -1.0f*shape->size.y, -1.0f*shape->size.z);  //h
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f*shape->size.x, 1.0f*shape->size.y, -1.0f*shape->size.z);  //d




	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f*shape->size.x, 1.0f*shape->size.y, 1.0f*shape->size.z);//b
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f*shape->size.x, -1.0f*shape->size.y, 1.0f*shape->size.z); //f
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f*shape->size.x, -1.0f*shape->size.y, 1.0f*shape->size.z); //e


	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f*shape->size.x, 1.0f*shape->size.y, 1.0f*shape->size.z);//b
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f*shape->size.x, -1.0f*shape->size.y, 1.0f*shape->size.z); //e
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f*shape->size.x, 1.0f*shape->size.y, 1.0f*shape->size.z);//a


	


	glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);

}

void SCube::InnerRender_Arrays(GLuint my_id, int num_vertices)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	// ... draw other buffers
	glDrawArrays(GL_TRIANGLES, 0, num_vertices * 3);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void SCube::DrawCubeGLDrawElements(GLuint my_id)
{
	GLuint g_Indices[24] = {
		0, 1, 2, 3,
		7, 4, 5, 6,
		6, 5, 2, 1,
		7, 0, 3, 4,
		7, 6, 1, 0,
		3, 2, 5, 4,
	};

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	// ... draw other buffers
	//glDrawArrays(GL_TRIANGLES, 0, num_vertices * 3);
	glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, &g_Indices[0]);
	glDisableClientState(GL_VERTEX_ARRAY);
}

Cube::Cube()
{

}

Cube::Cube(vec size)
{
	this->size.x = size.x;
	this->size.y = size.y;
	this->size.z = size.z;
}

Cube::~Cube()
{
}
