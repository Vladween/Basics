#pragma once
#ifndef OFFICE_BASICS_ELEMENT_H
#define OFFICE_BASICS_ELEMENT_H

#include "AppBasics/AppUpdatables.h"
#include "GraphicsBasics/RoundedRectangleShape.hpp"
#include "ElementEvent.h"

begin_basics_namespace(office)

class Element;

class RoundedRectangleState : public sf::RoundedRectangleShape
{
	friend class Element;
protected:
	using sf::RoundedRectangleShape::setPosition;
	using sf::RoundedRectangleShape::getPosition;
};

class Element : public Updatable, public sf::Drawable
{
public:
	RoundedRectangleState background;

	RepeatedElementEvent<void(Element*)> onMouseOver = RepeatedElementEvent<void(Element*)>(&onMouseOut),
		onMouseOut = RepeatedElementEvent<void(Element*)>(&onMouseOver);

	std::function<sf::Vector2f(AppInfo&)> usedMousePos = [](AppInfo& app) { return app.globalMousePos; };

	Element()
	{
		background.setCornerPointCount(10);
	}

	void setPosition(float x, float y)
	{
		setPosition({ x, y });
	}
	void setPosition(const sf::Vector2f& position)
	{
		m_position = position;
	}
	const sf::Vector2f& getPosition() const
	{
		return m_position;
	}
protected:
	void _element_update(AppInfo& app)
	{
		if (background.getPosition() != m_position)
			background.setPosition(m_position);

		if (background.getGlobalBounds().contains(usedMousePos(app))) onMouseOver(this);
		else onMouseOut(this);
	}

	void update(AppInfo& app) override
	{
		_element_update(app);
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override
	{
		target.draw(background, states);
	}
private:
	sf::Vector2f m_position;
};


end_basics_namespace(office)

#endif