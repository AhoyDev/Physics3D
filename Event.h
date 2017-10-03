#ifndef __EVENT_H__
#define __EVENT_H__

class Module;

enum EventType
{
	// User
	PLAY = 0,
	PAUSE,
	UNPAUSE,
	STOP,

	// System
	FILE_DROPPED,
	WINDOW_RESIZE,
	WINDOW_QUIT
};

struct Point
{
	int x, y;
};

union EventData
{
	const char* str;
	Point pos;
};

struct Event
{
	EventType type;
	EventData data;
	Module* listener = nullptr;
};

#endif