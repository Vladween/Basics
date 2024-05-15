#pragma once
#ifndef APP_BASICS_APPLICATION_H
#define APP_BASICS_APPLICATION_H

#include "AppUpdatables.h"

begin_basics_namespace(app)


class Application : public AppInfo,
	MainLoopUpdaters,  public Updater,
	EventLoopUpdaters, public EventUpdater,
	AppInitializers,   public Initializer
{
public:
	sf::ContextSettings windowSettings;

	void createWindow(sf::VideoMode windowMode, sf::String windowName, sf::Uint32 windowStyle = sf::Style::Default)
	{
		m_windowMode = windowMode;
		m_windowName = windowName;
		m_windowStyle = windowStyle;

		if (!m_window_created) return;

		window.create(m_windowMode, m_windowName, m_windowStyle, windowSettings);
		whenWindowCreated();
	}
	const sf::VideoMode& getWindowMode() const
	{
		return m_windowMode;
	}
	const sf::String& getWindowName() const
	{
		return m_windowName;
	}
	const sf::Uint32& getWindowStyle() const
	{
		return m_windowStyle;
	}

	void run()
	{
		MainLoopUpdaters::endInit();
		EventLoopUpdaters::endInit();
		AppInitializers::endInit();

		AppInitializers::processContainables(*this);
		init();

		window.create(m_windowMode, m_windowName, m_windowStyle, windowSettings);
		m_window_created = true;
		whenWindowCreated();

		while (window.isOpen())
		{
			cycleTime = cycleClock.restart();
			deltaTime = cycleTime.asMicroseconds() * timeRatio;

			localMousePos = sf::Mouse::getPosition(window);
			globalMousePos = window.mapPixelToCoords(localMousePos);

			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed && closeOnEvent) window.close();

				event_update();

				EventLoopUpdaters::processContainables(*this);
			}

			update();

			MainLoopUpdaters::processContainables(*this);

			window.setMouseCursor(cursor);
			window.clear(clearColor);
			draw();
			window.display();
		}
	}
protected:
	virtual void init()
	{
	}
	virtual void whenWindowCreated()
	{
	}
	virtual void event_update()
	{
	}
	virtual void update()
	{
	}
	virtual void draw()
	{
	}
private:
	sf::VideoMode m_windowMode;
	sf::String m_windowName;
	sf::Uint32 m_windowStyle = sf::Style::Default;
	bool m_window_created = false;
};


end_basics_namespace(app)

#endif // !APP_BASICS_APPLICATION_H
