#pragma once
#ifndef APP_BASICS_APPUPDATABLES_H
#define APP_BASICS_APPUPDATABLES_H

#include "AppInfo.h"
#include "Containable.h"

begin_basics_namespace(app)


class MainLoopPusher : public Containable<MainLoopPusher, AppInfo&>
{
public:
	using Containable<MainLoopPusher, AppInfo&>::Containable;
};

using MainLoopUpdaters = Container<MainLoopPusher, AppInfo&>;

template<class T>
class MainLoopUpdatable : public Containable<T, AppInfo&>
{
public:
	using Containable<T, AppInfo&>::Containable;
};

template<class T>
class MainLoopUpdater : public Container<T, AppInfo&>, public MainLoopPusher
{
	static_assert(std::is_base_of<MainLoopUpdatable<T>, T>::value, "Class 'T' must be derived from class 'MainLoopUpdatable<T>'!");
public:
	MainLoopUpdater(std::function<void(AppInfo&)> process = nullptr)
		: Container<T, AppInfo&>(process), MainLoopPusher([&](AppInfo& app) { Container<T, AppInfo&>::processContainables(app); })
	{
	}
};

class Updatable : public MainLoopUpdatable<Updatable>
{
public:
	Updatable()
		: MainLoopUpdatable<Updatable>([&](AppInfo& app) { update(app); })
	{
	}
protected:
	virtual void update(AppInfo&) = 0;
};
using Updater = MainLoopUpdater<Updatable>;


class EventLoopPusher : public Containable<EventLoopPusher, AppInfo&>
{
public:
	using Containable<EventLoopPusher, AppInfo&>::Containable;
};

using EventLoopUpdaters = Container<EventLoopPusher, AppInfo&>;

template<class T>
class EventLoopUpdatable : public Containable<T, AppInfo&>
{
public:
	using Containable<T, AppInfo&>::Containable;
};

template<class T>
class EventLoopUpdater : public Container<T, AppInfo&>, public EventLoopPusher
{
	static_assert(std::is_base_of<EventLoopUpdatable<T>, T>::value, "Class 'T' must be derived from 'EventLoopUpdatable<T>'!");
public:
	EventLoopUpdater(std::function<void(AppInfo&)> process = nullptr)
		: Container<T, AppInfo&>(process), EventLoopPusher([&](AppInfo& app) { Container<T, AppInfo&>::processContainables(app); })
	{
	}
};


class EventUpdatable : public EventLoopUpdatable<EventUpdatable>
{
public:
	EventUpdatable()
		: EventLoopUpdatable<EventUpdatable>([&](AppInfo& app) { event_update(app); })
	{
	}
protected:
	virtual void event_update(AppInfo&) = 0;
};
using EventUpdater = EventLoopUpdater<EventUpdatable>;



class InitPusher : public Containable<InitPusher, AppInfo&>
{
public:
	using Containable<InitPusher, AppInfo&>::Containable;
};

using AppInitializers = Container<InitPusher, AppInfo&>;

template<class T>
class AppInitializable : public Containable<T, AppInfo&>
{
public:
	using Containable<T, AppInfo&>::Containable;
};

template<class T>
class AppInitializer : public Container<T, AppInfo&>, public InitPusher
{
	static_assert(std::is_base_of<AppInitializable<T>, T>::value, "Class 'T' msut be derived from class 'AppInitializable<T>'!");
public:
	AppInitializer(std::function<void(AppInfo&)> process = nullptr)
		: Container<T, AppInfo&>(process), InitPusher([&](AppInfo& app) { Container<T, AppInfo&>::processContainables(app); })
	{
	}
};


class Initializable : public AppInitializable<Initializable>
{
public:
	Initializable()
		: AppInitializable([&](AppInfo& app) { init(app); })
	{
	}
protected:
	virtual void init(AppInfo&) = 0;
};

using Initializer = AppInitializer<Initializable>;

end_basics_namespace(app)

#endif // !APP_BASICS_APPUPDATABLES_H
