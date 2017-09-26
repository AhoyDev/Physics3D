#pragma once
#include <windows.h>
#include <stdio.h>
#include "imgui\imgui.h"

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)
#define MIN(a,b) ((a)<(b)) ? (a) : (b)
#define MAX(a,b) ((a)>(b)) ? (a) : (b)

#define RELEASE( x )\
    {\
       if( x != nullptr )\
       {\
         delete x;\
	     x = nullptr;\
       }\
    }

#define RELEASE_ARRAY( x )\
	{\
       if( x != nullptr )\
       {\
           delete[] x;\
	       x = nullptr;\
		 }\
	 }

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define HAVE_M_PI

#define KILOBYTE 1024
#define MEGABYTE 1048576
#define GIGABYTE 1073741824
#define KILOBYTE_F 1024.0
#define MEGABYTE_F 1048576.0
#define GIGABYTE_F 1073741824.0

typedef unsigned int uint;

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Configuration -----------
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 1024
#define SCREEN_SIZE 1
#define WIN_FULLSCREEN false
#define WIN_RESIZABLE true
#define WIN_BORDERLESS false
#define WIN_FULLSCREEN_DESKTOP false
#define VSYNC false
#define TITLE "3D Physics Playground"



struct Colors
{
	ImVec4 white = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 Black = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	ImVec4 yellow = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
	ImVec4 skyBlue = ImVec4(0.10f, 0.10f, 1.00f, 1.0f);
	ImVec4 blue = ImVec4(0.3f, 0.0f, 0.0f, 1.0f);
	ImVec4 darkBlue = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	ImVec4 pink = ImVec4(1.0f, 0.0f, 1.0f, 1.0f);
	ImVec4 Green = ImVec4(0, 0.88f, 0.73f, 1.00f);
	ImVec4 LighterGreen = ImVec4(0.5f, 1.0f, 0.5f, 1.0f);
	ImVec4 DarkerGreen = ImVec4(0.05f, 0.4f, 0.05f, 1.0f);
	ImVec4 AlphaGreen = ImVec4(0.05f, 1.0f, 0.05f, 1.0f);
	ImVec4 red = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);

};