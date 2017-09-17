#ifndef __TIMER_H__
#define __TIMER_H__

#include "SDL\include\SDL.h"

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

#endif //__TIMER_H__