#include "Scene.h"

#include "AssetManager.h"

Scene::Scene() :
	m_backgroundTexture(nullptr)
{
}

Scene::Scene(const std::string & textureName, const std::wstring & speaker, const std::wstring & text) :
	m_speaker(speaker), m_text(text)
{
	m_backgroundTexture = AssetManager::get<sf::Texture>(textureName);
	if (m_backgroundTexture != nullptr) {
		m_background = textureName;
	}
}

Scene::~Scene()
{
}

void Scene::setBackground(const std::string & textureName)
{
	m_backgroundTexture = AssetManager::get<sf::Texture>(textureName);
	if (m_backgroundTexture != nullptr) {
		m_background = textureName;
	}
}

std::string Scene::getBackground() const
{
	return m_background;
}

sf::Texture * Scene::getBackgroundTexture() const
{
	return m_backgroundTexture;
}

void Scene::setSpeaker(const std::wstring& speaker)
{
	m_speaker = speaker;
}

std::wstring Scene::getSpeaker() const
{
	return m_speaker;
}

void Scene::setText(const std::wstring & text)
{
	m_text = text;
}

std::wstring Scene::getText() const
{
	return m_text;
}
