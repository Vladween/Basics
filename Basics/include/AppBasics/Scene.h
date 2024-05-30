#pragma once
#ifndef APP_BASICS_SCENE_H
#define APP_BASICS_SCENE_H

#include "AppUpdatables.h"
#include "SystemBasics/Manager.h"

begin_basics_namespace(app)

class SceneManager;

class Scene : public Managable<Scene>, public Updater, public EventUpdater, public Initializer, public sf::Drawable
{
	friend class SceneManager;
public:
	Scene()
		: Updater([&](AppInfo& app)
			{
				if (!m_isRunning) return;

				update(app);

				for (auto updatable : Updater::containables())
					Updater::processContainable(updatable, app);
			}),
		EventUpdater([&](AppInfo& app)
			{
				if (!m_isRunning) return;

				event_update(app);

				for (auto event_updatable : EventUpdater::containables())
					EventUpdater::processContainable(event_updatable, app);
			}),
		Initializer([&](AppInfo& app)
			{
				for (auto initializable : Initializer::containables())
					Initializer::processContainable(initializable, app);

				init(app);
			})
	{
	}

	void bindToManager(Manager<Scene>& manager)
	{
		Managable<Scene>::_currentContainer(manager);
	}
	void unbindFromManager()
	{
		Manager<Scene>::Erase(this);
	}
	const SceneManager* getCurrentManager() const
	{
		return (const SceneManager*)Managable<Scene>::_currentContainer();
	}

	void start()
	{
		m_isRunning = true;
		setAsCurrent();
	}
	void stop()
	{
		m_isRunning = false;
		reset();

		Manager<Scene>* manager = (Manager<Scene>*)Managable<Scene>::_currentContainer();
		if (!is_valid(manager)) return;

		manager->setNextAsCurrent();
	}
	const bool& isRunning() const
	{
		return m_isRunning;
	}
protected:
	virtual void reset() {}
	virtual void init(AppInfo& app) {}
	virtual void event_update(AppInfo& app) {}
	virtual void update(AppInfo& app) {}
	virtual void _draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const {}

	void startInit()
	{
		Updater::startInit();
		EventUpdater::startInit();
		Initializer::startInit();
	}
	void endInit()
	{
		Updater::endInit();
		EventUpdater::endInit();
		Initializer::endInit();
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override final
	{
		if (!m_isRunning) return;

		_draw(target, states);
	}
private:
	using Managable<Scene>::setAsCurrent;
	using Updater::startInit;      using Updater::endInit;
	using EventUpdater::startInit; using EventUpdater::endInit;
	using Initializer::startInit;  using Initializer::endInit;

	bool m_isRunning = false;
};

class SceneManager : public Manager<Scene>
{
public:
	using Manager<Scene>::startInit;
	using Manager<Scene>::endInit;

	SceneManager()
		: Manager<Scene>([]() {}, nullptr, nullptr, [&](Scene* next_scene)
			{
				Scene* current_scene = (Scene*)getCurrentManagable();

				current_scene->m_isRunning = false;
				current_scene->reset();
			})
	{
	}

	bool start(Scene& scene)
	{
		int index = find(scene);

		if (index == -1) return false;

		containables()[index]->start();

		return true;
	}
	void start(size_t index)
	{
		if (index >= containables().size()) return;

		containables()[index]->start();
	}
	void startNext()
	{
		setNextAsCurrent();
	}
	void startPrevious()
	{
		setPreviousAsCurrent();
	}
private:
	using Manager<Scene>::setNextAsCurrent;
	using Manager<Scene>::setPreviousAsCurrent;
};

end_basics_namespace(app)

#endif // !APP_BASICS_SCENE_H
