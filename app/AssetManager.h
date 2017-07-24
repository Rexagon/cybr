#pragma once

#include <functional>
#include <memory>
#include <string>
#include <map>

#include "Asset.h"
#include "TextureAsset.h"
#include "ShaderAsset.h"
#include "ScriptAsset.h"
#include "MusicAsset.h"
#include "FontAsset.h"

class AssetManager
{
public:
	// ��������� ���������� � �������� �� json �����
	/// '����" ��������� - �����
	/// '������' ��������� - �����
	/// ���� ������ ������������ �� ���� ��� ���������� �����
	/// ��� �������������� ���� ��������� ������������ �����, � ������� ����� json ����
	static void load(const std::string& path);

	// ����������� ������� ������� � ��� ����������� ��������
	template<class T, class... Args>
	static void bind(const std::string& name, Args&&... factoryArguments)
	{
		auto it = m_factories.find(name);
		if (it == m_factories.end()) {
			m_factories[name] = std::unique_ptr<T>(new T(std::forward<Args>(factoryArguments)...));
		}
	}

	// ���������� ������� ������� �� ��� ��������
	static void unbind(const std::string& name);

	// ���������� ������ �� ��� ����������� ��������
	//@ ���� ������ ���, �� nullptr
	template<class T>
	static T* get(const std::string& name) {
		auto it = m_factories.find(name);
		if (it == m_factories.end()) {
			Log::write("unable to find resource: \"" + name + "\"");
			return nullptr;
		}
		else {
			return reinterpret_cast<T*>(it->second->load()->getData());
		}
	}

	static void clear(const std::string& name);

	static void clear();
private:
	static std::map<std::string, std::unique_ptr<AssetFactory>> m_factories;
};
