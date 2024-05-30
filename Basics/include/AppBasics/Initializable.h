#pragma once
#ifndef APP_BASICS_INITIALIZABLE_H
#define APP_BASICS_INITIALIZABLE_H

#include "AppInfo.h"
#include "SystemBasics/Container.h"

begin_basics_namespace(app)

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

	void bindToInitializer(AppInitializer<Initializable>& initializer)
	{
		_currentContainer(initializer);
	}
	void unbindFromInitializer()
	{
		AppInitializer<Initializable>::Erase(this);
	}
	const AppInitializer<Initializable>* getCurrentInitializer() const
	{
		return (const AppInitializer<Initializable>*)_currentContainer();
	}
protected:
	virtual void init(AppInfo&) = 0;
private:
	using AppInitializable<Initializable>::_currentContainer;
};

using Initializer = AppInitializer<Initializable>;

end_basics_namespace(app);

#endif // !APP_BASICS_INITIALIZABLE_H
