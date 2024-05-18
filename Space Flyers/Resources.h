#pragma once
#ifndef SPACE_FLYERS_RESOURCES_H
#define SPACE_FLYERS_RESOURCES_H

#include "SFMLBasics/Graphics.h"

static sf::Texture background_texture, buttons_texture, meteor_texture, player_texture;
static sf::Font font;

void init_resources()
{
	background_texture.loadFromFile("resources/background.jpg");
	buttons_texture.loadFromFile("resources/buttons.png");
	meteor_texture.loadFromFile("resources/meteor.png");
	player_texture.loadFromFile("resources/player.png");
	font.loadFromFile("resources/font.ttf");
}

#endif