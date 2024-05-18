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

#endif