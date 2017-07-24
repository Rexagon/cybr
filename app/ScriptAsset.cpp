#include "ScriptAsset.h"

#include <fstream>

ScriptFactory::ScriptFactory(const std::string & path) :
	m_path(path), m_asset(nullptr)
{
}

ScriptFactory::~ScriptFactory()
{
	if (m_asset != nullptr) {
		Log::write("script deleted: \"" + m_path + "\"");
	}
}

Asset * ScriptFactory::load()
{
	if (m_asset == nullptr) {
		std::shared_ptr<ScriptAsset> asset = std::make_shared<ScriptAsset>();
		
		std::ifstream file(m_path);
		if (file.is_open()) {
			asset->m_script = std::string(
				(std::istreambuf_iterator<char>(file)),
				std::istreambuf_iterator<char>());

			m_asset = std::move(asset);
			Log::write("script loaded: \"" + m_path + "\"");
		}
		else {
			throw std::runtime_error("unable to load script: \"" + m_path + "\"");
		}
	}

	return m_asset.get();
}

void ScriptFactory::clear()
{
	if (m_asset != nullptr) {
		Log::write("script deleted: \"" + m_path + "\"");
	}
	m_asset.reset();
}
