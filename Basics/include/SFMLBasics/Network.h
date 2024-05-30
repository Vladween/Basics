#pragma once
#ifndef SFML_BASICS_NETWORK_H
#define SFML_BASICS_NETWORK_H

#include "System.h"

#if _DEBUG == 1
#pragma comment(lib, "sfml-network-d.lib")
#else
#pragma comment(lib, "sfml-network.lib")
#endif

#include "SFML/Network.hpp"


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