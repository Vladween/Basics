#pragma once
#ifndef GAME_BASICS_COLLIDER_H
#define GAME_BASICS_COLLIDER_H

#include "Object.h"

begin_basics_namespace(game)

class BASICS_API Collider : public Object
{
public:
	Collider(sf::FloatRect bounds = sf::FloatRect())
		: Object(Object::Collider, bounds)
	{
	}
	virtual ~Collider()
	{
	}
protected:
	void resolveCollision(const Object* other) override
	{
		sf::FloatRect intersection;
		if (other->getBounds().intersects(this->getBounds(), intersection))
		{
			if (intersection.width < intersection.height)
			{
				int dir = (getPosition().x < other->getPosition().x) ? -1 : 1;
				if (other->getType() == Object::Collider) move(intersection.width / 2 * dir, 0);

				onCollision(other, { dir, 0 });
			}
			else if (intersection.height < intersection.width)
			{
				int dir = (getPosition().y < other->getPosition().y) ? -1 : 1;
				if (other->getType() == Object::Collider) move(0, intersection.height / 2 * dir);

				onCollision(other, { 0, dir });
			}
		}
	}

	virtual void onCollision(const Object* other, sf::Vector2i dir)
	{
	}
};

end_basics_namespace(game)

#endif