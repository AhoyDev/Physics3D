#ifndef __JSONDATAMANAGER_H__
#define __JSONDATAMANAGER_H__

#include "Globals.h"
#include "JSON\parson.h"

class JSONNode
{
public:
	JSONNode(const char* file = nullptr);
	JSONNode(JSON_Object* root);
	~JSONNode();

	// Push
	bool		PushBool(const char* name, const bool value);
	bool		PushInt(const char* name, const int value);
	bool		PushUInt(const char* name, const uint value);
	bool		PushFloat(const char* name, const float value);
	bool		PushDouble(const char* name, const double value);
	bool		PushString(const char* name, const char* value);
	JSONNode	PushJObject(const char* name);

	// Pull
	bool		PullBool(const char* name, bool deflt) const;
	int			PullInt(const char* name, int deflt) const;
	uint		PullUInt(const char* name, uint deflt) const;
	float		PullFloat(const char* name, float deflt) const;
	double		PullDouble(const char* name, double deflt) const;
	const char*	PullString(const char* name, const char* deflt) const;
	JSONNode	PullJObject(const char* name) const;

	// Utility
	uint Serialize(char** buffer, bool pretty = true);
	inline bool operator!() const;

public:

	JSON_Value*		value;
	JSON_Object*	object;
};

#endif