#pragma once

#include <SFML/Graphics.hpp>

#include "State.h"

#include "GUI.h"

class MainMenu : public State
{
public:
	MainMenu();
	~MainMenu();

	void init() override;
	void close() override;

	void update(const float dt) override;
	void draw(const float dt) override;

private:
	std::shared_ptr<Label> createMenuButton(const std::wstring& text);

	sf::Shader* m_blurShader;

	sf::Sprite m_background;

	sf::VertexArray m_cover;
	float m_coverOffset;
	int m_coverDirection;

	sf::RenderTexture m_renderTexture;
	sf::RenderTexture m_tempRenderTexture;

	bool m_changingState;
};