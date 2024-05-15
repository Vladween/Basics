#pragma once
#ifndef OFFICE_BASICS_LABEL_H
#define OFFICE_BASICS_LABEL_H

#include "Element.h"
#include "Align.h"

begin_basics_namespace(office)

class Label;

class TextState : public sf::Text
{
	friend class Label;
public:
	void setAlign(const Align& align)
	{
		m_align = align;
	}
	const Align& getAlign()
	{
		return m_align;
	}
private:
	Align m_align;
};

class Label : public Element
{
public:
	TextState text;

	Label()
	{
		background.setFillColor(sf::Color::Transparent);
	}
protected:
	void resolveTextAlign(TextState& text, sf::FloatRect bounds)
	{
		text.m_align.resolve(text, bounds);
	}

	void _label_update(AppInfo& app)
	{
		_element_update(app);

		if (background.getSize() == sf::Vector2f(0, 0) && text.getGlobalBounds().getSize() != sf::Vector2f(0, 0))
			background.setSize(text.getGlobalBounds().getSize());

		text.m_align.resolve(text, background.getGlobalBounds());
	}

	void update(AppInfo& app) override
	{
		_label_update(app);
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override
	{
		target.draw(background);
		target.draw(text);
	}
};

end_basics_namespace(office)

#endif // !OFFICE_BASICS_LABEL_H
