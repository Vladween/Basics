#pragma once
#ifndef GAME_BASICS_RIGIDBODY_H
#define GAME_BASICS_RIGIDBODY_H

#include "Object.h"

begin_basics_namespace(game)

class BASICS_API RigidBody : public Object
{
public:
	RigidBody(sf::FloatRect bounds = sf::FloatRect())
		: Object(Object::RigidBody, bounds)
	{
	}
	virtual ~RigidBody()
	{
	}
protected:
	void update(AppInfo& app) override
	{
	}

	void resolveCollision(const Object* other) override final
	{
		sf::FloatRect intersection;
		if (getBounds().intersects(other->getBounds(), intersection))
		{
			if (intersection.width < intersection.height)
			{
				int dir = (getPosition().x < other->getPosition().x) ? -1 : 1;
				if (other->getType() == Object::Collider) Object::move(intersection.width * dir - (0.01f * dir), 0);
				else if (other->getType() == Object::RigidBody) Object::move(intersection.width / 2 * dir - (0.01f * dir), 0);

				onCollision(other, { dir, 0 });
			}
			if (intersection.height < intersection.width)
			{
				int dir = (getPosition().y < other->getPosition().y) ? -1 : 1;
				if (other->getType() == Object::Collider) Object::move(0, intersection.height * dir - (0.01f * dir));
				else if (other->getType() == Object::RigidBody) Object::move(0, intersection.height / 2 * dir - (0.01f * dir));

				onCollision(other, { 0, dir });
			}
		}
	}

	virtual void onCollision(const Object* other, sf::Vector2i dir)
	{
	}
};

class BASICS_API PlatformerRigidBody : public RigidBody
{
public:
	void setGravity(sf::Vector2f gravity)
	{
		m_gravity = gravity;
	}
	sf::Vector2f getGravity() const
	{
		return m_gravity;
	}

	void setVelocity(sf::Vector2f velocity)
	{
		m_velocity = velocity;
	}
	sf::Vector2f getVelocity() const
	{
		return m_velocity;
	}

	sf::Vector2<bool> isOnGround() const
	{
		return m_onGround;
	}
	sf::Vector2i getLastCollisionDir() const
	{
		return m_last_collision_dir;
	}

	void move(float x, float y)
	{
		if (x != 0 && !(x < 0 && m_last_collision_dir.x == 1) || (x > 0 && m_last_collision_dir.x == -1))
			m_velocity.x = x;
		if (y != 0 && !(y < 0 && m_last_collision_dir.y == 1) || (y > 0 && m_last_collision_dir.y == -1))
			m_velocity.y = y;
	}
	void move(sf::Vector2f factor)
	{
		move(factor.x, factor.y);
	}
protected:
	void setOnGround(sf::Vector2<bool> onGround)
	{
		m_onGround = onGround;
	}
	void setLastCollisionDir(sf::Vector2i last_collision_dir)
	{
		m_last_collision_dir = last_collision_dir;
	}

	void update(AppInfo& app) override
	{
		platf_update(app);
	}

	void platf_update(AppInfo& app)
	{
		if (!m_onGround.x)
			m_velocity.x += m_gravity.x * app.deltaTime;
		if (!m_onGround.y)
			m_velocity.y += m_gravity.y * app.deltaTime;

		if ((m_velocity.x < 0 && m_last_collision_dir.x == 1) || (m_velocity.x > 0 && m_last_collision_dir.x == -1))
			m_velocity.x = 0;
		if ((m_velocity.y < 0 && m_last_collision_dir.y == 1) || (m_velocity.y > 0 && m_last_collision_dir.y == -1))
			m_velocity.y = 0;

		Object::move(m_velocity);

		if (m_gravity.x == 0)
			m_velocity.x = 0;
		if (m_gravity.y == 0)
			m_velocity.y = 0;

		m_onGround = { false, false };
		m_last_collision_dir = {};
	}

	void onCollision(const Object* obj, sf::Vector2i dir) override
	{
		onPlatfCollision(obj, dir);
	}

	void onPlatfCollision(const Object* obj, sf::Vector2i dir)
	{
		if (obj->getType() != Object::Collider) return;

		if ((m_gravity.x < 0 && dir.x == 1) || (m_gravity.x > 0 && dir.x == -1))
			m_onGround.x = true;
		if ((m_gravity.y < 0 && dir.y == 1) || (m_gravity.y > 0 && dir.y == -1))
			m_onGround.y = true;

		if (m_last_collision_dir.x == 0)
			m_last_collision_dir.x = dir.x;
		if (m_last_collision_dir.y == 0)
			m_last_collision_dir.y = dir.y;
	}
private:
	sf::Vector2f m_velocity, m_gravity;
	sf::Vector2<bool> m_onGround;
	sf::Vector2i m_last_collision_dir;
};

end_basics_namespace(game)

#endif