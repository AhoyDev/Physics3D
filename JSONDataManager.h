#ifndef __JSONDATAMANAGER_H__
#define __JSONDATAMANAGER_H__

#include "Globals.h"
#include "JSON\parson.h"

class JSONDataManager
{
public:
	JSONDataManager(const char* file = nullptr);
	~JSONDataManager();

	// Push
	json_result_t PushBool(const char* name, const bool value);
	json_result_t PushInt(const char* name, const int value);
	json_result_t PushUInt(const char* name, const uint value);
	json_result_t PushFloat(const char* name, const float value);
	json_result_t PushDouble(const char* name, const double value);
	json_result_t PushString(const char* name, const char* value);

	// Pull
	bool		PullBool(const char* name)const;
	int			PullInt(const char* name)const;
	uint		PullUInt(const char* name)const;
	float		PullFloat(const char* name)const;
	double		PullDouble(const char* name)const;
	const char*	PullString(const char* name)const;

public:

	JSON_Value*		value;
	JSON_Object*	object;
};

#endif