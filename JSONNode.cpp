#include "JSONNode.h"

#include "SDL\include\SDL_assert.h"

JSONNode::JSONNode(const char* file) : value(nullptr), object(nullptr)
{
	if (file != nullptr)
	{
		value = json_parse_string(file);
		object = json_value_get_object(value);
	}
	else
	{
		value = json_value_init_object();
		object = json_value_get_object(value);
	}
}

JSONNode::~JSONNode()
{
	json_value_free(value);
}

JSONNode::JSONNode(JSON_Object* root) : value(nullptr), object(root)
{}

// Push ================================================================================
bool JSONNode::PushBool(const char* name, const bool value)
{
	return json_object_set_boolean(object, name, (int)value) == JSONSuccess;
}

bool JSONNode::PushInt(const char* name, const int value)
{
	return json_object_set_number(object, name, (double)value) == JSONSuccess;
}

bool JSONNode::PushUInt(const char* name, const uint value)
{
	return json_object_set_number(object, name, (double)value) == JSONSuccess;
}

bool JSONNode::PushFloat(const char* name, const float value)
{
	return json_object_set_number(object, name, (double)value) == JSONSuccess;
}

bool JSONNode::PushDouble(const char* name, const double value)
{
	return json_object_set_number(object, name, value) == JSONSuccess;
}

bool JSONNode::PushString(const char* name, const char* value)
{
	return json_object_set_string(object, name, value) == JSONSuccess;
}

JSONNode JSONNode::PushJObject(const char * name)
{
	json_object_set_value(object, name, json_value_init_object());
	return PullJObject(name);
}

// Pull ================================================================================
bool JSONNode::PullBool(const char* name, bool deflt)const
{
	SDL_assert(name != nullptr);
	bool ret = deflt;
	JSON_Value* value = json_object_get_value(object, name);
	if (value && json_value_get_type(value) == JSONBoolean)
		ret = json_value_get_boolean(value) != 0;
	return ret;
}

int JSONNode::PullInt(const char* name, int deflt)const
{
	SDL_assert(name != nullptr);
	int ret = deflt;
	JSON_Value* value = json_object_get_value(object, name);
	if (value && json_value_get_type(value) == JSONNumber)
		ret = (int)json_value_get_number(value);
	return ret;
}

uint JSONNode::PullUInt(const char* name, uint deflt)const
{
	SDL_assert(name != nullptr);
	uint ret = deflt;
	JSON_Value* value = json_object_get_value(object, name);
	if (value && json_value_get_type(value) == JSONNumber)
		ret = (uint)json_value_get_number(value);
	return ret;
}

float JSONNode::PullFloat(const char* name, float deflt)const
{
	SDL_assert(name != nullptr);
	float ret = deflt;
	JSON_Value* value = json_object_get_value(object, name);
	if (value && json_value_get_type(value) == JSONNumber)
		ret = (float)json_value_get_number(value);
	return ret;
}

double JSONNode::PullDouble(const char* name, double deflt)const
{
	SDL_assert(name != nullptr);
	double ret = deflt;
	JSON_Value* value = json_object_get_value(object, name);
	if (value && json_value_get_type(value) == JSONNumber)
		ret = json_value_get_number(value);
	return ret;
}

const char* JSONNode::PullString(const char* name, const char* deflt)const
{
	SDL_assert(name != nullptr);
	const char* ret = NULL;
	JSON_Value* value = json_object_get_value(object, name);
	if (value && json_value_get_type(value) == JSONString)
		ret = json_value_get_string(value);
	return ret != NULL? ret : deflt;
}

JSONNode JSONNode::PullJObject(const char* name)const
{
	return JSONNode(json_object_get_object(object, name));
}

uint JSONNode::Serialize(char** buffer, bool pretty)
{
	uint size_ret = 0;

	if (pretty)
	{
		size_ret = json_serialization_size_pretty(value);
		*buffer = new char[size_ret];
		json_serialize_to_buffer_pretty(value, *buffer, size_ret);
		
	}
	else
	{
		size_ret = json_serialization_size(value);
		*buffer = new char[size_ret];
		json_serialize_to_buffer(value, *buffer, size_ret);
	}
	
	return size_ret;
}

inline bool JSONNode::operator!() const
{
	return object != nullptr;
}