#pragma once
#ifndef APP_BASICS_UPDATABLE_H
#define APP_BASICS_UPDATABLE_H

#include "AppInfo.h"
#include "SystemBasics/Container.h"

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

	void bindToUpdater(MainLoopUpdater<Updatable>& updater)
	{
		_currentContainer(updater);
	}
	void unbindFromUpdater()
	{
		MainLoopUpdater<Updatable>::Erase(this);
	}
	const MainLoopUpdater<Updatable>* getCurrentUpdater() const
	{
		return (const MainLoopUpdater<Updatable>*)_currentContainer();
	}
protected:
	virtual void update(AppInfo&) = 0;
private:
	using MainLoopUpdatable<Updatable>::_currentContainer;
};
using Updater = MainLoopUpdater<Updatable>;

end_basics_namespace(app)

#endif // !APP_BASICS_UPDATABLE_H
