#pragma once

#include <string>

#include <selene.h>

class ScriptManager
{
public:
	static void init();

	static void evaluate(const std::string& script);

	static sel::Selector get(const std::string& name);

	template<class T>
	static T get(const std::string& name)
	{
        return static_cast<T>(m_state[name.c_str()]);
	}
private:
	static sel::State m_state;
};
