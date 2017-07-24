#include "Intro.h"

#include "Core.h"

#include "MainMenu.h"

Intro::Intro() :
	m_alpha(0.0f), m_logo(nullptr)
{
	sf::Texture* logoTexture = AssetManager::get<sf::Texture>("main_logo");
	m_logo = std::make_unique<sf::Sprite>(*logoTexture);
	m_logo->setOrigin(vec2(logoTexture->getSize()) / 2.0f);
	m_logo->setPosition(vec2(Core::getWindow()->getSize()) * 0.5f);
}

Intro::~Intro()
{
	AssetManager::unbind("main_logo");
}

void Intro::init()
{
    SoundManager::setMusic("music_1");
	//SoundManager::playMusic();
}

void Intro::close()
{
}

void Intro::update(const float dt)
{
	m_alpha += dt;
	if (m_alpha >= math::PI || 
		Input::getKeyDown(Key::Space) ||
		Input::getKeyDown(Key::Return) ||
		Input::getMouseDown(MouseButton::Left) ||
		Input::getMouseDown(MouseButton::Right)) 
	{
		Core::changeState<MainMenu>();
		return;
	}

	if (Input::getKeyDown(Key::Escape)) {
		Core::stop();
		return;
	}
}

void Intro::draw(const float dt)
{
	Core::getWindow()->clear(sf::Color(230, 230, 230));

	m_logo->setColor(sf::Color(255, 255, 255, sin(m_alpha) * 255));
	Core::draw(*m_logo);
}
