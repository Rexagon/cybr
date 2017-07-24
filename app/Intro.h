#pragma once

#include <memory>

#include "State.h"

#include <SFML/Graphics.hpp>

class Intro : public State
{
public:
	Intro();
	~Intro();

	void init() override;
	void close() override;

	void update(const float dt) override;
	void draw(const float dt) override;

private:
	std::unique_ptr<sf::Sprite> m_logo;
	float m_alpha;
};