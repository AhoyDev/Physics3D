#ifndef __MODULE_H__
#define __MODULE_H__

#include "Globals.h"

class JSONNode;
struct PhysBody3D;
struct Event;

class Module
{
private :

	bool enabled;
	const char* name;

public:

	Module(const char* name, bool start_enabled = true) : name(name), enabled(start_enabled)
	{}

	virtual ~Module()
	{}

	bool IsEnabled() const
	{
		return enabled;
	}

	void Enable()
	{
		if(enabled == false)
		{
			enabled = true;
			Start();
		}
	}

	void Disable()
	{
		if(enabled == true)
		{
			enabled = false;
			CleanUp();
		}
	}

	virtual bool Init()
	{
		return true; 
	}

	virtual bool Start()
	{
		return true;
	}

	virtual update_status PreUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}

	const char* GetName()
	{
		return name;
	}

	virtual void Save(JSONNode* config) const
	{}

	virtual void Load(JSONNode* config)
	{}

	virtual void OnCollision(PhysBody3D* body1, PhysBody3D* body2)
	{}

	virtual void ReceiveEvent(const Event& event)
	{}

	virtual void DrawDebug()
	{}
};

#endif