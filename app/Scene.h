#pragma once

#include <SFML/Graphics.hpp>

class Scene
{
public:
	Scene();
	Scene(const std::string& textureName, const std::wstring& speaker, const std::wstring& text);
	~Scene();

	void setBackground(const std::string& textureName);
	std::string getBackground() const;
	sf::Texture* getBackgroundTexture() const;
	
	void setSpeaker(const std::wstring& speaker);
	std::wstring getSpeaker() const;

	void setText(const std::wstring& text);
	std::wstring getText() const;
private:
	std::string m_background;
	sf::Texture* m_backgroundTexture;

	std::wstring m_speaker;
	std::wstring m_text;
};