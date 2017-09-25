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
	json_result_t	PushBool(const char* name, const bool value);
	json_result_t	PushInt(const char* name, const int value);
	json_result_t	PushUInt(const char* name, const uint value);
	json_result_t	PushFloat(const char* name, const float value);
	json_result_t	PushDouble(const char* name, const double value);
	json_result_t	PushString(const char* name, const char* value);
	JSONNode		PushJObject(const char* name);

	// Pull
	bool		PullBool(const char* name) const;
	int			PullInt(const char* name) const;
	uint		PullUInt(const char* name) const;
	float		PullFloat(const char* name) const;
	double		PullDouble(const char* name) const;
	const char*	PullString(const char* name) const;
	JSONNode	PullJObject(const char* name) const;

	bool isValid() const;
	uint Serialize(char** buffer, bool pretty = true);

public:

	JSON_Value*		value;
	JSON_Object*	object;

	bool validNode;
};

#endif