#pragma once

#include <memory>
#include <string>

#include "Asset.h"

class ScriptAsset : public Asset
{
public:
	void* getData() override
	{
		return &m_script;
	}

private:
	friend class ScriptFactory;

	std::string m_script;
};

class ScriptFactory : public AssetFactory
{
public:
	ScriptFactory(const std::string& path);
	~ScriptFactory();

	Asset* load() override;
	void clear() override;

private:
	std::string m_path;
	std::shared_ptr<ScriptAsset> m_asset;
};