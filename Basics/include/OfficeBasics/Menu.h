#pragma once
#ifndef OFFICE_BASICS_MENU_H
#define OFFICE_BASICS_MENU_H

#include "AppBasics/AppUpdatables.h"

begin_basics_namespace(office)

class Menu : public Updater, public EventUpdater, public sf::Drawable
{
public:
	Menu()
		: Updater([&](AppInfo& app)
			{
				if (!m_isOpen) return;
				update(app);
				for (auto updatable : this->Updater::containables())
					this->Updater::processContainable(updatable, app);
			}),
		EventUpdater([&](AppInfo& app)
			{
				if (!m_isOpen) return;
				event_update(app);
				for (auto updatable : this->EventUpdater::containables())
					this->EventUpdater::processContainable(updatable, app);
			})
	{}
	sf::RectangleShape background;

	void open()
	{
		m_isOpen = true;
	}
	void close()
	{
		m_isOpen = false;
		reset();
	}
	bool isOpen() const
	{
		return m_isOpen;
	}
protected:
	virtual void reset()
	{
	}
	virtual void event_update(AppInfo& app)
	{
	}
	virtual void update(AppInfo& app)
	{
	}

	virtual void _draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const = 0;

	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override final
	{
		if (!m_isOpen) return;

		target.draw(background);
		_draw(target, states);
	}
private:
	bool m_isOpen = false;
};

end_basics_namespace(office)

#endif