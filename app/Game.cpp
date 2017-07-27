#include "Game.h"

#include "Core.h"

Game::Game() :
    m_blurShader(nullptr), m_scrolling(0),
    m_guiState(0), m_coverOffset(0.0f), m_cover(sf::Quads, 4)
{
	vec2 windowSize = vec2(Core::getWindow()->getSize());

	// Инициализация шейдера размытия
	m_blurShader = AssetManager::get<sf::Shader>("shader_blur");
	m_blurShader->setUniform("resolution", static_cast<float>(windowSize.x));
	m_blurShader->setUniform("radius", 2.0f);

	// Временные буфферы для размытия
    m_renderTexture.create(windowSize.x, windowSize.y);
    m_tempRenderTexture.create(windowSize.x, windowSize.y);

	// Инициализация фона
	m_background.setTexture(sf::Texture());
	m_background.setTextureRect(recti(0, 0, windowSize.x, windowSize.y));

	// Создание "ширмы"
	sf::Color coverColor = sf::Color(180, 180, 180);

	float diff = 60.0f;

	m_cover[0].position = vec2(-diff - windowSize.x / 4.0f, 0.0f);
	m_cover[0].color = coverColor;

	m_cover[1].position = vec2(windowSize.x, 0.0f);
	m_cover[1].color = coverColor;

	m_cover[2].position = vec2(windowSize.x + diff, windowSize.y);
	m_cover[2].color = coverColor;

	m_cover[3].position = vec2(-windowSize.x / 4.0f, windowSize.y);
	m_cover[3].color = coverColor;

	
	// Создание интерфейса

	vec2 labelTextSize(600.0f, 90.0f);

	// Создание поля с именем говорящего
	m_labelName = m_gui.create<Label>();
	m_labelName->setPosition((windowSize.x - labelTextSize.x) / 2.0f, windowSize.y - labelTextSize.y - 30.0f);
	m_labelName->setSize(labelTextSize.x, 30.0f);
	m_labelName->setFontSize(18); // 「」
	m_labelName->setAlignment(GUI::AlignLeft);
	m_labelName->setPadding(10.0f);
	m_labelName->setBackgroundColor(sf::Color(100, 100, 100, 255));
	m_gui.getRootWidget()->getLayout()->addWidget(m_labelName);

	// Создание поля с текстом говорящего
	m_labelText = m_gui.create<Label>();
	m_labelText->setPosition((windowSize.x - labelTextSize.x) / 2.0f, windowSize.y - labelTextSize.y);
	m_labelText->setSize(labelTextSize);
	m_labelText->setFontSize(16);
	m_labelText->setAlignment(GUI::AlignLeft | GUI::AlignTop);
	m_labelText->setPadding(10.0f);
	m_labelText->setBackgroundColor(sf::Color(128, 128, 128, 255));
	m_labelText->setWordwrapEnabled(true);
	m_gui.getRootWidget()->getLayout()->addWidget(m_labelText);

	// Создание кнопок навигации между сценами
	float buttonsWidth = 51.0f;

	sf::Color buttonsTextColor = sf::Color(255, 255, 255, 160);

	auto hoverFunction = [buttonsTextColor](Widget* widget) {
		sf::Color hoveredColor = buttonsTextColor;
		hoveredColor.a = 220;
		reinterpret_cast<Label*>(widget)->setColor(hoveredColor);
	};

	auto unhoverFunction = [buttonsTextColor](Widget* widget) {
		reinterpret_cast<Label*>(widget)->setColor(buttonsTextColor);
	};

	// Кнопка назад
	m_buttonLeft = m_gui.create<Label>();
	m_buttonLeft->setPosition((windowSize.x - labelTextSize.x) / 2.0f - buttonsWidth, windowSize.y - labelTextSize.y);
	m_buttonLeft->setSize(buttonsWidth, labelTextSize.y);
	m_buttonLeft->setFontSize(50);
	m_buttonLeft->setColor(buttonsTextColor);
	m_buttonLeft->setBackgroundColor(sf::Color::Transparent);
	m_buttonLeft->setText(L"〈");
	m_buttonLeft->bind(Widget::Hover, hoverFunction);
	m_buttonLeft->bind(Widget::Unhover, unhoverFunction);
	m_buttonLeft->bind(Widget::Press, [this](Widget* widget) {
        moveBackward();
	});
	m_gui.getRootWidget()->getLayout()->addWidget(m_buttonLeft);

	// Кнопка вперёд
	m_buttonRight = m_gui.create<Label>();
	m_buttonRight->setPosition((windowSize.x + labelTextSize.x) / 2.0f, windowSize.y - labelTextSize.y);
	m_buttonRight->setSize(buttonsWidth, labelTextSize.y);
	m_buttonRight->setFontSize(50);
	m_buttonRight->setColor(buttonsTextColor);
	m_buttonRight->setBackgroundColor(sf::Color::Transparent);
	m_buttonRight->setText(L"〉");
	m_buttonRight->bind(Widget::Hover, hoverFunction);
	m_buttonRight->bind(Widget::Unhover, unhoverFunction);
	m_buttonRight->bind(Widget::Press, [this](Widget* widget) {
        moveForward();
	});
	m_gui.getRootWidget()->getLayout()->addWidget(m_buttonRight);

	// Кнопка паузы (вызов меню)
	m_buttonPause = m_gui.create<Label>();
	m_buttonPause->setPosition(0.0f, 0.0f);
	m_buttonPause->setSize(50.0f, 50.0f);
	m_buttonPause->setFontSize(30);
	m_buttonPause->setColor(buttonsTextColor);
	m_buttonPause->setBackgroundColor(sf::Color::Transparent);
	m_buttonPause->setText(L"⏸"); // ||
	m_buttonPause->bind(Widget::Hover, hoverFunction);
	m_buttonPause->bind(Widget::Unhover, unhoverFunction);
	m_buttonPause->bind(Widget::Press, [this](Widget* widget) {

	});
	m_gui.getRootWidget()->getLayout()->addWidget(m_buttonPause);

    // Меню
    m_menuWidget = m_gui.create<Widget>();
    m_menuLayout = new Layout(m_menuWidget);
    m_gui.getRootWidget()->getLayout()->addWidget(m_menuWidget);
}

Game::~Game()
{
}

void Game::init()
{
    m_guiState = 1;
	m_coverOffset = 0.0f;

	loadScript();
}

void Game::close()
{
}

void Game::update(const float dt)
{
    if (m_guiState == 0 || m_guiState == 4) {
        int mouseWheelDelta = Input::getMouseWheelDelta();
        int scroll = 0;
        if (m_scrolling * mouseWheelDelta < 0 || m_scrolling == 0) {
            scroll = mouseWheelDelta;
        }
        m_scrolling = mouseWheelDelta;

        if (Input::getKeyDown(Key::Return) ||
            Input::getKeyDown(Key::Space) ||
            Input::getKeyDown(Key::Right) ||
            scroll < 0)
        {
            if (m_guiState == 0) {
                moveForward();
            }
        }

        if (Input::getKeyDown(Key::BackSpace) ||
            Input::getKeyDown(Key::Left) ||
            scroll > 0)
        {
            if (m_guiState == 4) {
                closeMenu();
            }

            moveBackward();
        }
    }
    else if (m_guiState == 1) {
        m_coverOffset += dt * 3000.0f;
    }
    else if (m_guiState == 2) {
        m_coverOffset -= dt * 3000.0f;
    }
    else if (m_guiState == 3) {
        // TODO: pause menu
    }
    /*else if (m_guiState == 4) {
        // TODO: menu selection
    }*/

	if (Input::getKeyDown(Key::F5)) {
		loadScript();
	}

	if (Input::getKeyDown(Key::Escape)) {
        m_guiState = 2;
		m_coverOffset = Core::getWindow()->getSize().x;
		return;
    }
}

void Game::draw(const float dt)
{
	float windowWidth = Core::getWindow()->getSize().x;

	Core::draw(m_background);

    sf::RenderStates renderStates;
    renderStates.shader = m_blurShader;

    m_blurShader->setUniform("dir", vec2(1.0, 0.0));
    m_tempRenderTexture.clear();
    m_tempRenderTexture.draw(m_background, renderStates);
    m_tempRenderTexture.display();

    m_blurShader->setUniform("dir", vec2(0.0, 1.0));
    m_renderTexture.clear();
    sf::Sprite sprite(m_tempRenderTexture.getTexture());
    m_renderTexture.draw(sprite, m_blurShader);
    m_renderTexture.display();

    if (m_guiState == 0) {
        m_labelName->setTexture(&m_renderTexture.getTexture());
        m_labelText->setTexture(&m_renderTexture.getTexture());
        m_gui.draw();
    }
    else if (m_guiState == 1 || m_guiState == 2) {
        renderStates.shader = nullptr;
        renderStates.transform.translate(m_coverOffset, 0.0f);
        renderStates.texture = &m_renderTexture.getTexture();

        for (unsigned int i = 0; i < m_cover.getVertexCount(); ++i) {
            m_cover[i].texCoords = m_cover[i].position + vec2(m_coverOffset, 0.0f);
        }

        Core::draw(m_cover, renderStates);

        if (m_guiState == 1 && m_coverOffset >= windowWidth * 1.25f + 100.0f) {
            m_guiState = 0;
            m_coverOffset = 0.0f;
            return;
        }
        else if (m_guiState == 2 && m_coverOffset <= 0.0f) {
            Core::deleteState();
            return;
        }
    }
    else if (m_guiState == 3) {

    }
    else if (m_guiState == 4) {
        for (unsigned int i = 0; i < m_menuItems.size(); ++i) {
            m_menuItems[i]->setTexture(&m_renderTexture.getTexture());
        }
        m_gui.draw();
    }
}

void Game::loadScript()
{
	AssetManager::clear("script_game");

	ScriptManager::get("Engine")["Log"] = [](const std::string& text) { Log::write(text); };

	ScriptManager::get("Engine")["SetBackground"] = [this](const std::string& textureName) {
		this->setBackground(textureName);
	};
	ScriptManager::get("Engine")["SetDialogName"] = [this](const std::string& name) {
		this->setDialogName(name);
	};
	ScriptManager::get("Engine")["SetDialogText"] = [this](const std::string& text) {
		this->setDialogText(text);
	};
    ScriptManager::get("Engine")["ShowMenu"] = [this]() {
        this->showMenu();
    };
    ScriptManager::get("Engine")["CloseMenu"] = [this]() {
        this->closeMenu();
    };
    ScriptManager::get("Engine")["AddMenuItem"] = [this](const std::string& label) {
        this->addMenuItem(label);
    };
    ScriptManager::get("Engine")["GetSelectedItem"] = [this]() {
        return this->getSelectedItem();
    };
    ScriptManager::get("Engine")["IsMenuOpened"] = [this]() {
        return this->isMenuOpened();
    };

	ScriptManager::evaluate(*AssetManager::get<std::string>("script_game"));

    ScriptManager::get("Core")["Update"]();
}

void Game::setBackground(const std::string & textureName)
{
	sf::Texture* texture = AssetManager::get<sf::Texture>(textureName);
	if (texture != nullptr) {
		m_background.setTexture(*texture);
	}
}

void Game::setDialogName(const std::string & name)
{
	m_labelName->setText(name);
}

void Game::setDialogText(const std::string & text)
{
    m_labelText->setText(text);
}

void Game::moveForward()
{
    ScriptManager::get("Core")["Forward"]();
    ScriptManager::get("Core")["Update"]();
}

void Game::moveBackward()
{
    ScriptManager::get("Core")["Backward"]();
    ScriptManager::get("Core")["Update"]();
}

void Game::showMenu()
{
    vec2 windowSize = vec2(Core::getWindow()->getSize());

    vec2 itemPosition((windowSize.x - 600.0f) / 2.0f, (windowSize.y - 41.0f * m_menuItems.size()) / 2.0f);
    for (unsigned int i = 0; i < m_menuItems.size(); ++i) {
        m_menuItems[i]->setPosition(itemPosition + vec2(0.0f, i * 41.0f));
        m_menuItems[i]->setVisible(true);
    }

    m_buttonLeft->setVisible(false);
    m_buttonRight->setVisible(false);
    m_labelName->setVisible(false);
    m_labelText->setVisible(false);
    m_guiState = 4;
}

void Game::closeMenu()
{
    for (auto& item : m_menuItems) {
        m_gui.prepareDeleting(item);
    }
    m_menuItems.clear();
    m_menuLayout = new Layout(m_menuWidget);
    m_selectedItem = 0;

    m_buttonLeft->setVisible(true);
    m_buttonRight->setVisible(true);
    m_labelName->setVisible(true);
    m_labelText->setVisible(true);
    m_guiState = 0;
}

bool Game::isMenuOpened() const
{
    return m_guiState == 4;
}

void Game::addMenuItem(const std::string& label)
{
    std::shared_ptr<Label> menuItem = m_gui.create<Label>();
    menuItem->setText(label);
    menuItem->setSize(600.0f, 40.0f);
    menuItem->setFontSize(16);
    menuItem->setAlignment(GUI::AlignCenter);
    menuItem->setPadding(10.0f);
    menuItem->setBackgroundColor(sf::Color(100, 100, 100, 255));
    menuItem->setColor(sf::Color(255, 255, 255, 160));
    menuItem->bind(Widget::Hover, [](Widget* widget) {
        reinterpret_cast<Label*>(widget)->setColor(sf::Color(255, 255, 255, 220));
    });
    menuItem->bind(Widget::Unhover, [](Widget* widget) {
        reinterpret_cast<Label*>(widget)->setColor(sf::Color(255, 255, 255, 180));
    });
    int index = m_menuItems.size() + 1;
    menuItem->bind(Widget::Press, [this, index](Widget* widget) {
        this->m_selectedItem = index;
        ScriptManager::evaluate("coroutine.resume(Core.Settings.Handlers.Menu)");
        this->closeMenu();
    });
    m_menuLayout->addWidget(menuItem);
    m_menuItems.push_back(menuItem.get());
}

int Game::getSelectedItem() const
{
    return m_selectedItem;
}
