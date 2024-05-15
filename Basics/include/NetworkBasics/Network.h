#pragma once
#ifndef NETWORK_BASICS_CONFIG_H
#define NETWORK_BASICS_CONFIG_H

#include "SFML/Network.hpp"
#include "AppBasics/AppUpdatables.h"

#if _DEBUG == 1
#pragma comment(lib, "sfml-network-d.lib")
#else
#pragma comment(lib, "sfml-network.lib")
#endif

template<typename T>
sf::Packet& operator<<(sf::Packet& packet, sf::Vector2<T> vec2)
{
	return packet << vec2.x << vec2.y;
}
template<typename T>
sf::Packet& operator>>(sf::Packet& packet, sf::Vector2<T>& vec2)
{
	return packet >> vec2.x >> vec2.y;
}

template<typename T>
sf::Packet& operator<<(sf::Packet& packet, sf::Vector3<T> vec3)
{
	return packet << vec3.x << vec3.y << vec3.z;
}

template<typename T>
sf::Packet& operator>>(sf::Packet& packet, sf::Vector3<T> vec3)
{
	return packet >> vec3.x >> vec3.y >> vec3.z;
}

#endif