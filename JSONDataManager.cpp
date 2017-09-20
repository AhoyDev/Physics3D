#include "JSONDataManager.h"

#include "SDL\include\SDL_assert.h"

JSONDataManager::JSONDataManager(const char* file)
{
	SDL_assert(file != nullptr);
	value = json_value_init_object();
	object = json_value_get_object(value);
}

JSONDataManager::~JSONDataManager()
{
	json_value_free(value);
}

// Push ================================================================================
json_result_t JSONDataManager::PushBool(const char* name, const bool value)
{
	SDL_assert(name != nullptr);
	return json_result_t(json_object_set_boolean(object, name, (int)value));
}

json_result_t JSONDataManager::PushInt(const char* name, const int value)
{
	SDL_assert(name != nullptr);
	return json_result_t(json_object_set_number(object, name, (double)value));
}

json_result_t JSONDataManager::PushUInt(const char* name, const uint value)
{
	SDL_assert(name != nullptr);
	return json_result_t(json_object_set_number(object, name, (double)value));
}

json_result_t JSONDataManager::PushFloat(const char* name, const float value)
{
	SDL_assert(name != nullptr);
	return json_result_t(json_object_set_number(object, name, (double)value));
}

json_result_t JSONDataManager::PushDouble(const char* name, const double value)
{
	SDL_assert(name != nullptr);
	return json_result_t(json_object_set_number(object, name, value));
}

json_result_t JSONDataManager::PushString(const char* name, const char* value)
{
	SDL_assert(name != nullptr && value != nullptr);
	return json_result_t(json_object_set_string(object, name, value));
}

// Pull ================================================================================
bool JSONDataManager::PullBool(const char* name)const
{
	SDL_assert(name != nullptr);
	return json_object_get_boolean(object, name) > 0;
}

int JSONDataManager::PullInt(const char* name)const
{
	SDL_assert(name != nullptr);
	return (int)json_object_get_number(object, name);
}

uint JSONDataManager::PullUInt(const char* name)const
{
	SDL_assert(name != nullptr);
	return (uint)json_object_get_number(object, name);
}

float JSONDataManager::PullFloat(const char* name)const
{
	SDL_assert(name != nullptr);
	return (float)json_object_get_number(object, name);
}

double JSONDataManager::PullDouble(const char* name)const
{
	SDL_assert(name != nullptr);
	return json_object_get_number(object, name);
}

const char* JSONDataManager::PullString(const char* name)const
{
	SDL_assert(name != nullptr);
	return json_object_get_string(object, name);
}
