#pragma once
#ifndef SFML_BASICS_SYSTEM_H
#define SFML_BASICS_SYSTEM_H

#include "SystemBasics/Config.h"

#if _DEBUG == 1
#pragma comment(lib, "sfml-system-d.lib")
#else
#pragma comment(lib, "sfml-system.lib")
#endif

#include "SFML/System.hpp"

template<typename T>
std::ostream& operator<<(std::ostream& stream, const sf::Vector2<T>& vec2)
{
	return stream << "{" << vec2.x << ";" << vec2.y << "}";
}
template<typename T>
std::ostream& operator<<(std::ostream& stream, const sf::Vector3<T>& vec3)
{
	return stream << "{" << vec3.x << ";" << vec3.y << ";" << vec3.z << "}";
}


template<typename T>
std::istream& operator>>(std::istream& stream, sf::Vector2<T>& vec2)
{
	return stream >> vec2.x >> vec2.y;
}
template<typename T>
std::istream& operator>>(std::istream& stream, sf::Vector3<T>& vec3)
{
	return stream >> vec3.x >> vec3.y >> vec3.z;
}


namespace sf
{
	template<typename T>
	T length(const sf::Vector2<T>& vec2)
	{
		return (T)sqrtf(vec2.x * vec2.x + vec2.y * vec2.y);
	}
	template<typename T>
	T length(const sf::Vector3<T>& vec3)
	{
		return (T)sqrtf(vec3.x * vec3.x + vec3.y * vec3.y + vec3.z * vec3.z);
	}

	template<typename T>
	T normalized(const T& vec)
	{
		return vec / length(vec);
	}

	template<typename T>
	T distance(const sf::Vector2<T>& vec1, const sf::Vector2<T>& vec2)
	{
		return (T)sqrtf(powf(vec2.x - vec1.x, 2) + powf(vec2.y - vec1.y, 2));
	}
	template<typename T>
	T distance(const sf::Vector3<T>& vec1, const sf::Vector3<T>& vec2)
	{
		return (T)sqrtf(powf(vec2.x - vec1.x, 2) + powf(vec2.y - vec1.y, 2) + powf(vec2.z - vec1.z, 2));
	}
}

#endif