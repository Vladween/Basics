#pragma once
#ifndef GAME_BASICS_TRIGGER_H
#define GAME_BASICS_TRIGGER_H

#include "Object.h"

begin_basics_namespace(game)

class BASICS_API Trigger : public Object
{
public:
	Trigger(sf::FloatRect bounds = sf::FloatRect())
		: Object(Object::Trigger, bounds)
	{
	}
	virtual ~Trigger()
	{
	}
protected:
	virtual void onEnter(const Object*) {}
	virtual void onStay(const Object*) {}
	virtual void onExit(const Object*) {}
	virtual void onFirstEnter(const Object* other) { onEnter(other); }
	virtual void onLastExit(const Object* other) { onExit(other); }

	void resolveCollision(const Object* other) override final
	{
		auto other_itr = std::find(m_current.begin(), m_current.end(), other);
		bool other_found = other_itr != m_current.end();

		if (other->getBounds().intersects(this->getBounds()))
		{
			if (!other_found)
			{
				m_current.push_back(other);
				if (m_current.size() == 1) onFirstEnter(other);
				else onEnter(other);
			}
			else onStay(other);
		}
		else if (other_found)
		{
			if (m_current.size() == 1) onLastExit(other);
			else onExit(other);
			m_current.erase(other_itr);
		}
	}
private:
	std::vector<const Object*> m_current;
};

end_basics_namespace(game)

#endif