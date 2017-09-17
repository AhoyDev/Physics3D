#include "Timer.h"

// ---------------------------------------------
Timer::Timer(bool start_active)
{
	running = false;

	if (start_active)
		Start();
}

// ---------------------------------------------
void Timer::Start()
{
	running = true;
	started_at = SDL_GetTicks();
}

// ---------------------------------------------
void Timer::Stop()
{
	running = false;
	stopped_at = SDL_GetTicks();
}

// ---------------------------------------------
Uint32 Timer::Read()
{
	return running ? SDL_GetTicks() - started_at : stopped_at - started_at;
}







/** Alarm Mechanic
* \brief Compare SDL ticks values, and return true if A has passed B
*
* e.g. if you want to wait 100 ms, you could do this:
*  Uint32 timeout = SDL_GetTicks() + 100;
*  while (!SDL_TICKS_PASSED(SDL_GetTicks(), timeout)) {
*      ... do work until timeout has elapsed
*  }
*/
//#define SDL_TICKS_PASSED(A, B)  ((Sint32)((B) - (A)) <= 0)