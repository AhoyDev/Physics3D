#ifndef __TIMEMANAGER_H__
#define __TIMEMANAGER_H__

#include <list>
#include "SDL\include\SDL.h"

class Timer;

class TimeManager
{
public:

	TimeManager();
	~TimeManager();

	Timer*	CreateTimer(bool start_active = true);
	void	RemoveTimer(Timer* timer = nullptr);

	void	ManageFrameTimers();
	float	UpdateDeltaTime();
	void	SetMaxFPS(int max);

	float	GetDeltaTime() const;
	int		GetFpsCounter() const;
	float	GetLastFrameMs() const;
	int		GetLastFPS() const;
	int		GetCappedMS() const;

public:
	
	Uint32	frames;
	float	dt;
	int		fps_counter;
	float	last_frame_ms;
	int		last_fps;
	float	capped_ms;

private:

	Timer*	ms_timer;
	Timer*	fps_timer;

	std::list<Timer*> timers;
};

class Timer
{
public:

	// Constructor
	Timer(bool start_active = true);

	void Start();
	void Stop();

	Uint32 Read();

private:

	bool	running;
	Uint32	started_at;
	Uint32	stopped_at;

};

//class Alarm;

#endif