#pragma once
#include <windows.h>
#include <stdio.h>

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
	UPDATE_RESTART,
	UPDATE_STOP,
	UPDATE_ERROR
};