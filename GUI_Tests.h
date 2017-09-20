#ifndef __GUITESTS_H__
#define __GUITESTS_H__

#include "imgui/imgui.h"

#include "RandomGenerator.h"
#include "Primitives.h"
#include "R_Sphere.h"

class GUI_Tests
{
	//Methods
public:

	GUI_Tests();

	//Individual Tests
	void Test1();
	void Test2();
	void Test3();
	void Test4();

private:
	
	//Attributes
public:
	void ShowIntersectionMenu();

private:
	ImGuiWindowFlags flags;

	R_Sphere* sphere;
	R_Capsule* capsule;

	RandomGenerator* random_machine;
};




#endif // __GUI_Tests_H__