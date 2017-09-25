#include "JSONNode.h"

#include "SDL\include\SDL_assert.h"

JSONNode::JSONNode(const char* file)
{
	validNode = false;

	if (file != nullptr)
	{
		value = json_parse_string(file);
		object = json_value_get_object(value);

		if (value && object)
			validNode = true;
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
json_result_t JSONNode::PushBool(const char* name, const bool value)
{
	SDL_assert(name != nullptr);
	return json_result_t(json_object_set_boolean(object, name, (int)value));
}

json_result_t JSONNode::PushInt(const char* name, const int value)
{
	SDL_assert(name != nullptr);
	return json_result_t(json_object_set_number(object, name, (double)value));
}

json_result_t JSONNode::PushUInt(const char* name, const uint value)
{
	SDL_assert(name != nullptr);
	return json_result_t(json_object_set_number(object, name, (double)value));
}

json_result_t JSONNode::PushFloat(const char* name, const float value)
{
	SDL_assert(name != nullptr);
	return json_result_t(json_object_set_number(object, name, (double)value));
}

json_result_t JSONNode::PushDouble(const char* name, const double value)
{
	SDL_assert(name != nullptr);
	return json_result_t(json_object_set_number(object, name, value));
}

json_result_t JSONNode::PushString(const char* name, const char* value)
{
	SDL_assert(name != nullptr && value != nullptr);
	return json_result_t(json_object_set_string(object, name, value));
}

JSONNode JSONNode::PushJObject(const char * name)
{
	json_object_set_value(object, name, json_value_init_object());
	return PullJObject(name);
}

// Pull ================================================================================
bool JSONNode::PullBool(const char* name)const
{
	SDL_assert(name != nullptr);
	return json_object_get_boolean(object, name) > 0;
}

int JSONNode::PullInt(const char* name)const
{
	SDL_assert(name != nullptr);
	return (int)json_object_get_number(object, name);
}

uint JSONNode::PullUInt(const char* name)const
{
	SDL_assert(name != nullptr);
	return (uint)json_object_get_number(object, name);
}

float JSONNode::PullFloat(const char* name)const
{
	SDL_assert(name != nullptr);
	return (float)json_object_get_number(object, name);
}

double JSONNode::PullDouble(const char* name)const
{
	SDL_assert(name != nullptr);
	return json_object_get_number(object, name);
}

const char* JSONNode::PullString(const char* name)const
{
	SDL_assert(name != nullptr);
	return json_object_get_string(object, name);
}

JSONNode JSONNode::PullJObject(const char* name)const
{
	return JSONNode(json_object_get_object(object, name));
}

bool JSONNode::isValid() const
{
	return validNode;
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