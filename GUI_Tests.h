#ifndef __GUITESTS_H__
#define __GUITESTS_H__

#include "GUI_Window.h"

class GUI_Tests : public GUI_Window
{
public:

	GUI_Tests(const bool active = false);
	~GUI_Tests();

	void Draw();

	void Test();
};

#endif // __GUI_Tests_H__