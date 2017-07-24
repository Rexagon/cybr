#include "MainMenu.h"

#include "Core.h"

#include "Game.h"

MainMenu::MainMenu() :
	m_changingState(false), m_cover(sf::Quads, 4)
{
	vec2 windowSize = vec2(Core::getWindow()->getSize());

	// Инициализация шейдера размытия
	m_blurShader = AssetManager::get<sf::Shader>("shader_blur");
	m_blurShader->setUniform("resolution", static_cast<float>(windowSize.x));
	m_blurShader->setUniform("radius", 2.0f);

	// Временные буфферы для размытия
	m_renderTexture.create(windowSize.x, windowSize.y);
	m_tempRenderTexture.create(windowSize.x, windowSize.y);

	// Инициализация фона меню
	sf::Texture* backgroundTexture = AssetManager::get<sf::Texture>("menu_background");
	if (backgroundTexture != nullptr) {
		m_background.setTexture(*backgroundTexture);
	}

	// Создание "ширмы"
	sf::Color coverColor = sf::Color(180, 180, 180);

	float diff = 60.0f;

	m_cover[0].position = vec2(-windowSize.x, 0);
	m_cover[0].color = coverColor;

	m_cover[1].position = vec2(windowSize.x, 0);
	m_cover[1].color = coverColor;

	m_cover[2].position = vec2(windowSize.x + diff, windowSize.y);
	m_cover[2].color = coverColor;

	m_cover[3].position = vec2(-windowSize.x, windowSize.y);
	m_cover[3].color = coverColor;


	// Создание интерфейса

	// Версия приложения
	std::shared_ptr<Label> versionLabel = m_gui.create<Label>();
	versionLabel->setText("v0.01a");
	versionLabel->setFontSize(10);
	versionLabel->setPosition(windowSize - vec2(19.0f, 10.0f));
	m_gui.getRootWidget()->getLayout()->addWidget(versionLabel);

	// Обёртка для меню
	std::shared_ptr<Widget> menu = m_gui.create<Label>();
	menu->setPosition(80.0f, 400.0f);
	menu->setSize(200.0f, 145.0f);
	menu->setBackgroundColor(sf::Color::Transparent);
	m_gui.getRootWidget()->getLayout()->addWidget(menu);

	VerticalLayout* menuLayout = new VerticalLayout;
	menu->setLayout(menuLayout);

	// Кнопки меню
    std::shared_ptr<Label> buttonNewGame = createMenuButton(L"Новая игра");
	menuLayout->addWidget(buttonNewGame);
	buttonNewGame->bind(Widget::Press, [this](Widget* widget) {
		m_changingState = true;
	});

    std::shared_ptr<Label> buttonOptions = createMenuButton(L"Настройка");
	menuLayout->addWidget(buttonOptions);

    std::shared_ptr<Label> buttonExit = createMenuButton(L"Выход");
	buttonExit->bind(Widget::Press, [](Widget* widget) {
		Core::deleteState();
	});
	menuLayout->addWidget(buttonExit);
}

MainMenu::~MainMenu()
{
}

void MainMenu::init()
{
	m_coverDirection = 1.0;
	m_changingState = false;
	m_coverOffset = 290.0f;
}

void MainMenu::close()
{
}

void MainMenu::update(const float dt)
{
	if (m_changingState) {
		m_coverOffset += dt * m_coverDirection * 3000.0f;
	}

	if (Input::getKeyDown(Key::Escape)) {
		Core::deleteState();
		return;
	}
}

void MainMenu::draw(const float dt)
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

	if (m_coverOffset < 290.0f) {
		m_coverOffset = 290.0f;
	}

	renderStates.shader = nullptr;
	renderStates.transform.translate(m_coverOffset - windowWidth, 0.0f);
	renderStates.texture = &m_renderTexture.getTexture();

	for (unsigned int i = 0; i < m_cover.getVertexCount(); ++i) {
		m_cover[i].texCoords = m_cover[i].position + vec2(m_coverOffset - windowWidth, 0.0f);
	}

	Core::draw(m_cover, renderStates);

	if (m_changingState) {
		if (m_coverDirection > 0 && m_coverOffset >= windowWidth + 100.0f) {
			m_coverDirection = -1;
			Core::addState<Game>();
			return;
		}
		else if (m_coverDirection < 0 && m_coverOffset <= 290.0f) {
			m_changingState = false;
			m_coverOffset = 290.0f;
			m_coverDirection = 1;
		}
	}
	else {
		m_gui.draw();
	}
}

std::shared_ptr<Label> MainMenu::createMenuButton(const std::wstring & text)
{
	sf::Color m_buttonBackgroundColor = sf::Color(250, 250, 255, 150);

	std::shared_ptr<Label> button = m_gui.create<Label>();
	button->setText(text);
	button->setColor(sf::Color::Black);
	button->setFontSize(16);
	button->setFixedHeight(30.0f);
	button->setBackgroundColor(m_buttonBackgroundColor);
	button->bind(Widget::Hover, [](Widget* widget) {
		widget->setBackgroundColor(sf::Color(250, 250, 255, 200));
	});
	button->bind(Widget::Unhover, [m_buttonBackgroundColor](Widget* widget) {
		widget->setBackgroundColor(m_buttonBackgroundColor);
	});

	return button;
}
