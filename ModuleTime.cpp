#include "ModuleTime.h"

#include "Timer.h"

ModuleTime::ModuleTime(bool start_enabled) : Module(start_enabled)
{
	frames = 0;
	last_frame_ms = -1;
	last_fps = -1;
	capped_ms = 1000 / 60;
	fps_counter = 0;

	ms_timer = CreateTimer();
	fps_timer = CreateTimer();
}

ModuleTime::~ModuleTime()
{}

bool ModuleTime::CleanUp()
{
	bool ret = true;

	std::list<Timer*>::reverse_iterator i = timers.rbegin();
	for (; i != timers.rend(); ++i)
	{
		delete (*i);
	}

	timers.clear();

	return ret;
}

Timer* ModuleTime::CreateTimer(bool start_active)
{
	Timer* timer = new Timer(start_active);
	timers.push_back(timer);
	return timer;
}

void ModuleTime::ManageFrameTimers()
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

	last_frame_ms = ms_timer->Read();

	// cap fps
	if (last_frame_ms < capped_ms)
		SDL_Delay(capped_ms - last_frame_ms);
}

float ModuleTime::UpdateDeltaTime()
{
	dt = (float)ms_timer->Read() / 1000.0f;
	ms_timer->Start();

	return dt;
}

float ModuleTime::GetDeltaTime()
{
	return dt;
}



