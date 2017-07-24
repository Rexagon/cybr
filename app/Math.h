#pragma once

#include <vector>
#include <cmath>

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>
#include <SFML/Graphics/Rect.hpp>

using vec2 = sf::Vector2f;
using vec2i = sf::Vector2i;
using vec2u = sf::Vector2u;

using vec3 = sf::Vector3f;
using vec3i = sf::Vector3i;

using rect = sf::FloatRect;
using recti = sf::IntRect;

namespace math
{
	// ���������

	const float PI = 3.141592653f;
	const float E = 2.718281828f;


	// ��������� �������

	// ���������� ���� ���������
	template <typename T>
	int sign(T val)
	{
		return (T(0) < val) - (val < T(0));
	}

	// �������� value � ��������� [min, max]
	//@ ���� value > max, �� ����� max
	//@ ���� value < min, �� ����� min
	//@ ����� ����� value
	float clamp(float value, float min, float max);

	// ��������� �� �������� � �������
	float toRadians(float degrees);

	// ��������� �� ������ � �������
	float toDegrees(float radians);

	// �������� �������� ������� � x, �������� �������
	//@ ������������ �������� ������������
	float interpolate(const std::vector<std::pair<float, float>>& curve, float x);


	// ��������� �������

	// ���������� ����� �������
	float length(const vec2& v);

	// ��������� ������������ ����� ����� ���������
	float dot(const vec2& a, const vec2& b);

	// ���������� ��������������� ������
	vec2 normalize(const vec2& v);

	// ���������� ������ v, ��������� �� ���� angle
	//@ angle - ���� � ��������
	vec2 rotate(const vec2& v, float angle);

	// ���������� ����� �������� b �� a
	float scalarProjection(const vec2& a, const vec2& b);

	// ���������� ������ �������� b �� a
	vec2 vectorProjection(const vec2& a, const vec2& b);
}