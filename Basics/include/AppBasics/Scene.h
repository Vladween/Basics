#pragma once
#ifndef APP_BASICS_SCENE_H
#define APP_BASICS_SCENE_H

#include "AppUpdatables.h"

begin_basics_namespace(app)

class SceneManager;

class Scene : public MainLoopUpdatable<Scene>, public Updater, public EventUpdater , public sf::Drawable
{
public:
	friend class SceneManager;

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

				for (auto updatable : EventUpdater::containables())
					EventUpdater::processContainable(updatable, app);
			})
	{
	}

	bool isRunning() const
	{
		return m_isRunning;
	}
protected:
	void start()
	{
		onStart();
		m_isRunning = true;
	}
	void stop()
	{
		onStop();
		m_isRunning = false;
	}

	virtual void onStart()
	{
	}
	virtual void onStop()
	{
	}
	virtual void event_update(AppInfo& app)
	{
	}
	virtual void update(AppInfo& app)
	{
	}
	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override
	{
	}
private:
	bool m_isRunning = false;
};

class BASICS_API SceneManager : public MainLoopUpdater<Scene>, public sf::Drawable
{
public:
	SceneManager()
		: MainLoopUpdater<Scene>([&](AppInfo& app) { this->update(); })
	{
	}

	void start(size_t index)
	{
		if (containables().empty()) return;

		if (index >= containables().size()) index = containables().size() - 1;

		if (m_currentScene != nullptr) m_currentScene->stop();
		m_currentScene = containables()[index];
		m_currentScene->start();
	}
	void start(Scene& scene)
	{
		auto itr = std::find(containables().begin(), containables().end(), &scene);
		if (itr == containables().end()) return;

		if (m_currentScene != nullptr) m_currentScene->stop();
		m_currentScene = containables().at(itr - containables().begin());
		m_currentScene->start();
	}

	void start_next()
	{
		if (containables().empty()) return;

		if (m_currentScene == nullptr)
		{
			m_currentScene = containables()[0];
			m_currentScene->start();
			return;
		}

		m_currentScene->stop();
		size_t itr = std::find(containables().begin(), containables().end(), m_currentScene) - containables().begin() + 1;
		if (itr == containables().size()) itr = 0;

		m_currentScene = containables()[itr];
		m_currentScene->start();
	}
	void start_previous()
	{
		if (containables().empty()) return;

		if (m_currentScene == nullptr)
		{
			m_currentScene = containables().back();
			m_currentScene->start();
			return;
		}

		m_currentScene->stop();
		size_t itr = std::find(containables().begin(), containables().end(), m_currentScene) - containables().begin() - (size_t)1;
		if (itr < 0) itr = containables().size() - (size_t)1;

		m_currentScene = containables()[itr];
		m_currentScene->start();
	}
	const Scene* current()
	{
		return m_currentScene;
	}
protected:
	void update()
	{
		if (m_currentScene == nullptr)
		{
			if (containables().empty()) return;

			m_currentScene = containables()[0];
			m_currentScene->start();
		}

		if (!m_currentScene->isRunning())
			start_next();
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override
	{
		if (m_currentScene == nullptr) return;

		target.draw(*m_currentScene, states);
	}
private:
	Scene* m_currentScene = nullptr;
};

end_basics_namespace(app)

#endif // !APP_BASICS_SCENE_H
