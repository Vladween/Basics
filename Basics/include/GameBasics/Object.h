#pragma once
#ifndef GAME_BASICS_OBJECT_H
#define GAME_BASICS_OBJECT_H

#include "AppBasics/AppUpdatables.h"

begin_basics_namespace(game)

class ObjectWorld;

class BASICS_API Object : public MainLoopUpdatable<Object>, public sf::RectangleShape
{
public:
	friend class ObjectWorld;

	enum Type { Undefined = -1, Trigger = 0, RigidBody, Collider };

	Object(Type type = Undefined, sf::FloatRect bounds = sf::FloatRect())
		: m_type(type), m_bounds(bounds), MainLoopUpdatable<Object>([&](AppInfo&) {})
	{
	}
	virtual ~Object()
	{
	}

	void setPosition(sf::Vector2f position)
	{
		m_bounds.left = position.x;
		m_bounds.top = position.y;

		sf::RectangleShape::setPosition(m_bounds.getPosition());
	}
	void setPosition(float x, float y)
	{
		setPosition({ x, y });
	}
	sf::Vector2f getPosition() const
	{
		return m_bounds.getPosition();
	}

	void move(float x, float y)
	{
		m_bounds.left += x;
		m_bounds.top += y;

		sf::RectangleShape::setPosition(m_bounds.getPosition());
	}
	void move(sf::Vector2f factor)
	{
		move(factor.x, factor.y);
	}

	void setSize(sf::Vector2f size)
	{
		m_bounds.width = size.x;
		m_bounds.height = size.y;

		if (sf::RectangleShape::getSize() == sf::Vector2f{ 0, 0 })
			sf::RectangleShape::setSize(m_bounds.getSize());
	}
	sf::Vector2f getSize() const
	{
		return m_bounds.getSize();
	}

	void setRectangleSize(sf::Vector2f size)
	{
		sf::RectangleShape::setSize(size);
	}
	sf::Vector2f getRectangleSize() const
	{
		return sf::RectangleShape::getSize();
	}

	void setBounds(const sf::FloatRect& bounds)
	{
		setPosition(bounds.left, bounds.top);
		setSize({ bounds.width, bounds.height });
	}
	const sf::FloatRect& getBounds() const
	{
		return m_bounds;
	}
	sf::FloatRect* getBoundsPtr()
	{
		return &m_bounds;
	}

	Type getType() const
	{
		return m_type;
	}
protected:
	virtual void update(AppInfo& app)
	{
	}
	virtual void resolveCollision(const Object* other)
	{
	}
private:
	sf::FloatRect m_bounds;
	Type m_type;
};

class BASICS_API ObjectWorld : public MainLoopUpdater<Object>
{
public:
	ObjectWorld(std::function<void(AppInfo&)> onUpdate = nullptr)
		: MainLoopUpdater<Object>((onUpdate == nullptr) ? [&](AppInfo& app)
			{
				update_objects(app);
			} : onUpdate)
	{
	}

	void update_objects(AppInfo& app)
	{
		for (auto object : containables())
			object->update(app);

		for (size_t i = 0; i < containables().size() - 1; i++)
		{
			for (size_t j = i + 1; j < containables().size(); j++)
			{
				containables()[i]->resolveCollision(containables()[j]);
				containables()[j]->resolveCollision(containables()[i]);
			}
		}
	}
};

end_basics_namespace(game)

#endif