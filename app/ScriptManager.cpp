#include "ScriptManager.h"

#include "Log.h"

sel::State ScriptManager::m_state { true };

void ScriptManager::init()
{
	m_state.HandleExceptionsWith([](int code, std::string message, std::exception_ptr ptr) {
		Log::write("lua error:\n", message);
	});

	m_state("local oldreq = require\n\
require = function(s) return oldreq('data.scripts.' ..s) end");
}

void ScriptManager::evaluate(const std::string & script)
{
	m_state(script.c_str());
}

sel::Selector ScriptManager::get(const std::string & name)
{
	return m_state[name.c_str()];
}
