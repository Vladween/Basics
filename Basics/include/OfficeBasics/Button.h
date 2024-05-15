#pragma once
#ifndef OFFICE_BASICS_BUTTON_H
#define OFFICE_BASICS_BUTTON_H

#include "Label.h"
#include "Clickable.h"
#include <iostream>

begin_basics_namespace(office)

class Button : public Label, public EventUpdatable, Clickable<sf::Mouse::Button>
{
public:
	std::function<void(Button*, sf::Mouse::Button)> onClick = [](Button*, sf::Mouse::Button) {};
	ElementEvent<void(Button*, sf::Mouse::Button)> onMouseDown = ElementEvent<void(Button*, sf::Mouse::Button)>(&onMouseUp),
		onMouseUp = ElementEvent<void(Button*, sf::Mouse::Button)>(&onMouseDown);
protected:
	void _button_event_update(AppInfo& app)
	{
		if (background.getGlobalBounds().contains(usedMousePos(app)))
		{
			if (app.event.type == sf::Event::MouseButtonPressed)
				onMouseDown(this, app.event.mouseButton.button);
			else if (app.event.type == sf::Event::MouseButtonReleased)
				onMouseUp(this, app.event.mouseButton.button);

			bool needed_event = app.event.type == sf::Event::MouseButtonPressed || app.event.type == sf::Event::MouseButtonReleased;
			if (needed_event && clicked(sf::Mouse::isButtonPressed(app.event.mouseButton.button), app.event.mouseButton.button))
				onClick(this, app.event.mouseButton.button);
		}
	}
	void _button_update(AppInfo& app)
	{
		_label_update(app);
	}

	void event_update(AppInfo& app) override
	{
		_button_event_update(app);
	}
	void update(AppInfo& app) override
	{
		_button_update(app);
	}
};

end_basics_namespace(office)

#endif
