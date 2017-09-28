#include "TimeManager.h"

#include "Application.h"

TimeManager::TimeManager()
{
	frames = 0;
	last_frame_ms = -1;
	last_fps = -1;
	capped_ms = 1000 / 60;
	fps_counter = 0;

	ms_timer = CreateTimer();
	fps_timer = CreateTimer();
}

TimeManager::~TimeManager()
{
	std::list<Timer*>::reverse_iterator i = timers.rbegin();
	for (; i != timers.rend(); ++i)
	{
		delete (*i);
	}

	timers.clear();
}

Timer* TimeManager::CreateTimer(bool start_active)
{
	Timer* timer = new Timer(start_active);
	timers.push_back(timer);
	return timer;
}


void TimeManager::RemoveTimer(Timer* timer)
{
	if (timer != nullptr)
		timers.remove(timer);
}

void TimeManager::ManageFrameTimers()
{
	// Recap on framecount and fps
	++frames;
	++fps_counter;

	if (fps_timer->Read() >= 1000)
	{
		last_fps = fps_counter;
		fps_counter = 0;
		fps_timer->Start();
	}

	last_frame_ms = (float)ms_timer->Read();

	// cap fps
	if (capped_ms > 0 && last_frame_ms < capped_ms)
		SDL_Delay(capped_ms - last_frame_ms);
}

float TimeManager::UpdateDeltaTime()
{
	dt = (float)ms_timer->Read() / 1000.0f;
	ms_timer->Start();

	return dt;
}

void TimeManager::SetMaxFPS(int max)
{
	App->config_values.max_fps = max;

	if (max == 0)
		capped_ms = 0;
	else
		capped_ms = 1000.0f / (double)max;
}


float TimeManager::GetDeltaTime() const
{
	return dt;
}

int TimeManager::GetFpsCounter() const
{
	return fps_counter;
}

float TimeManager::GetLastFrameMs() const
{
	return last_frame_ms;
}

int TimeManager::GetLastFPS() const
{
	return last_fps;
}

int TimeManager::GetCappedMS() const
{
	return capped_ms;
}

// TIME =======================================================================================
Timer::Timer(bool start_active)
{
	running = false;

	if (start_active)
		Start();
}

void Timer::Start()
{
	running = true;
	started_at = SDL_GetTicks();
}

void Timer::Stop()
{
	running = false;
	stopped_at = SDL_GetTicks();
}

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