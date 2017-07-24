#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include "Math.h"

using Key = sf::Keyboard::Key;
using MouseButton = sf::Mouse::Button;

class Input
{
public:
	// ��������� ��������� ������
	static void update();

	// ������ �� ������� � ������� ������
	static bool getKey(Key keyCode);

	// ��������� �� ������� ������� � ���� �����
	static bool getKeyDown(Key keyCode);

	// ��������� �� ������� � ���� �����
	static bool getKeyUp(Key keyCode);


	// ������ �� ������ ���� � ������� ������
	static bool getMouse(MouseButton button);

	// ��������� �� ������� ������ ���� � ���� �����
	static bool getMouseDown(MouseButton button);

	// ��������� �� ������ ���� � ���� �����
	static bool getMouseUp(MouseButton button);

	// ������� ������� ������������ ������ �������� ���� ����
	static vec2 getMousePosition();

	// ���������� ����� ������� ����
	//@ ���� ������������, �� ������ ��������� �����
	//@ ���� �������������, �� ����
	static int getMouseWheelDelta();

private:
	friend class Core;

	static const int NUM_KEYS;
	static const int NUM_MOUSEBUTTONS;

	static vec2 m_mousePosition;
	static int m_mouseWheelDelta;

	static std::vector<bool> m_currentKeysState;
	static std::vector<bool> m_lastKeysState;

	static std::vector<bool> m_currentMouseButtonsState;
	static std::vector<bool> m_lastMouseButtonsState;
};