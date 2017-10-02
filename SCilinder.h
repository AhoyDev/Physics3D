#pragma once



#ifndef __SCILINDER_H__
#define __SCILINDER_H__

#include "SPrimitive.h"
#include "Glew\include\glew.h"
#include "Globals.h"

#define PI 3.1415


class Cilinder;

class SCilinder : public SPrimitive<Cilinder>
{
public:

	SCilinder(const GLfloat height, GLfloat  radius = 1.f);



	void DrawCilinder(GLfloat radius,
		GLfloat height,
		GLubyte R,
		GLubyte G,
		GLubyte B);


	GLfloat radius;
	GLubyte height;
	GLubyte R;
	GLubyte G;
	GLubyte B;




};

#endif



