#pragma once
#ifndef APP_BASICS_EVENT_UPDATABLE_H
#define APP_BASICS_EVENT_UPDATABLE_H

#include "AppInfo.h"
#include "SystemBasics/Container.h"

begin_basics_namespace(app)

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

	void bindToEventUpdater(EventLoopUpdater<EventUpdatable>& updater)
	{
		_currentContainer(updater);
	}
	void unbindFromEventUpdater()
	{
		EventLoopUpdater<EventUpdatable>::Erase(this);
	}
	const EventLoopUpdater<EventUpdatable>* getCurrentEventUpdater() const
	{
		return (const EventLoopUpdater<EventUpdatable>*)_currentContainer();
	}
protected:
	virtual void event_update(AppInfo&) = 0;
private:
	using EventLoopUpdatable<EventUpdatable>::_currentContainer;
};
using EventUpdater = EventLoopUpdater<EventUpdatable>;

end_basics_namespace(app)

#endif // !APP_BASICS_EVENT_UPDATABLE_H
