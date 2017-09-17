#ifndef __MODULETIME_H__
#define __MODULETIME_H__

#include "Module.h"
#include <list>
#include "SDL\include\SDL.h"

class Timer;

class ModuleTime : public Module
{
public:

	ModuleTime(bool start_enabled = true);
	~ModuleTime();
	
	bool CleanUp();

	Timer*	CreateTimer(bool start_active = true);
	void	ManageFrameTimers();
	float	UpdateDeltaTime();
	float	GetDeltaTime();

public:
	
	Uint32	frames;
	float	dt;
	int		fps_counter;
	int		last_frame_ms;
	int		last_fps;
	int		capped_ms;

private:

	Timer*	ms_timer;
	Timer*	fps_timer;

	std::list<Timer*> timers;
};

#endif