#pragma once
#ifndef SPACE_FLYERS_GAME_SCENE_H
#define SPACE_FLYERS_GAME_SCENE_H

#include "GameBasics.h"
#include "GraphicsBasics.h"

class Player : public basics::RigidBody, public basics::AnimationManager, public basics::Updatable
{
public:
	Player()
		: AnimationManager((sf::RectangleShape&)*this)
	{
	}
protected:
	void update(basics::AppInfo& app) override
	{

	}
private:
	basics::Animation fly_up, fly_down, fly_straight, crash;
};

class GameScene : public basics::GameScene
{
protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override
	{
		target.draw(player);
	}
private:
	Player player;
};

#endif