#pragma once

#include <SFML/Graphics.hpp>

#include "State.h"

#include "Scene.h"
#include "GUI.h"

namespace sel {
    class State;
}

class Game : public State
{
public:
	Game();
	~Game();

	void init() override;
	void close() override;

	void update(const float dt) override;
	void draw(const float dt) override;

private:
	void loadScript();

	void setBackground(const std::string& textureName);
	void setDialogName(const std::string& name);
	void setDialogText(const std::string& text);

    void moveForward();
    void moveBackward();

    void showMenu();
    void closeMenu();
    void addMenuItem(const std::string& label);
    int getSelectedItem();

    std::shared_ptr<Widget> m_menuWidget;
    Layout* m_menuLayout;
    std::vector<Label*> m_menuItems;

	std::shared_ptr<Label> m_buttonLeft;
	std::shared_ptr<Label> m_buttonRight;
	std::shared_ptr<Label> m_buttonPause;

	std::shared_ptr<Label> m_labelName;
	std::shared_ptr<Label> m_labelText;

    sf::Sprite m_background;

    sf::Shader* m_blurShader;
	sf::RenderTexture m_renderTexture;
	sf::RenderTexture m_tempRenderTexture;

	int m_scrolling;

    // 0 - обычный режим
    // 1 - ширма вправо
    // 2 - ширма влево
    // 3 - меню паузы
    // 4 - меню выбора
    int m_guiState;

    float m_coverOffset;
    sf::VertexArray m_cover;

    int m_selectedItem;
};
