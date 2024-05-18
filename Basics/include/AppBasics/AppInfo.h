#pragma once
#ifndef APP_BASICS_APPINFO_H
#define APP_BASICS_APPINFO_H

#include "SFMLBasics/Graphics.h"

begin_basics_namespace(app)

class BASICS_API RenderWindowState : public sf::RenderWindow
{
	friend class Application;
protected:
	using sf::RenderWindow::create;
};

class BASICS_API AppInfo
{
public:
	RenderWindowState window;
	sf::Color clearColor;
	sf::Event event = sf::Event();
	sf::Cursor cursor;
	sf::Vector2i localMousePos;
	sf::Vector2f globalMousePos;
	sf::Clock cycleClock;
	sf::Time cycleTime;
	float deltaTime = 0, timeRatio = 0.01f;
	bool closeOnEvent = true;

	AppInfo()
	{
		cursor.loadFromSystem(sf::Cursor::Arrow);
	}
};

end_basics_namespace(app);

#endif