#pragma once
#ifndef GRAPHICS_BASICS_ANIMATION_H
#define GRAPHICS_BASICS_ANIMATION_H

#include "AppBasics/AppUpdatables.h"

begin_basics_namespace(graphics)

class AnimationManager;

class Animation : public MainLoopUpdatable<Animation>
{
public:
	friend class AnimationManager;

	Animation()
		: MainLoopUpdatable<Animation>([&](AppInfo& app) { this->update(app); })
	{
	}

	bool isPlaying() const
	{
		return m_isPlaying;
	}

	void setFramesCount(size_t count)
	{
		m_frames.resize(count);
	}
	size_t getFramesCount() const
	{
		return m_frames.size();
	}

	void setFrame(size_t index, sf::IntRect rect)
	{
		if (index >= m_frames.size()) return;

		m_frames[index] = rect;
	}
	sf::IntRect getFrame(size_t index) const
	{
		if (index >= m_frames.size()) return {};

		return m_frames[index];
	}

	void setFrames(const std::vector<sf::IntRect>& frames)
	{
		m_frames = frames;
	}
	const std::vector<sf::IntRect>& getFrames()
	{
		return m_frames;
	}

	void setSpeed(float speed)
	{
		m_speed = speed;
	}
	float getSpeed() const
	{
		return m_speed;
	}

	void setMirrored(bool use_mirrored = true)
	{
		m_mirror = use_mirrored;
	}
	bool isMirrored() const
	{
		return m_mirror;
	}

	void setRepeated(bool repeat = true)
	{
		if (repeat) m_finished = false;

		m_repeat = repeat;
	}
	bool isRepeated() const
	{
		return m_repeat;
	}

	bool isFinished()
	{
		return m_finished;
	}

	sf::IntRect getCurrentFrame() const
	{
		if (!m_mirror) return getNormalCurrentFrame();
		return getMirroredCurrentFrame();
	}
	sf::IntRect getNormalCurrentFrame() const
	{
		return m_frames[(size_t)m_currentFrame];
	}
	sf::IntRect getMirroredCurrentFrame() const
	{
		return sf::IntRect(m_frames[(size_t)m_currentFrame].left + m_frames[(size_t)m_currentFrame].width, m_frames[(size_t)m_currentFrame].top,
			-m_frames[(size_t)m_currentFrame].width, m_frames[(size_t)m_currentFrame].height);
	}
	size_t getCurrentFrameIndex() const
	{
		return (size_t)m_currentFrame;
	}
protected:
	void update(AppInfo& app)
	{
		if (!m_isPlaying) return;

		m_currentFrame += m_speed * app.deltaTime;

		if ((size_t)m_currentFrame >= m_frames.size())
		{
			if (m_repeat) m_currentFrame -= m_frames.size();
			else m_finished = true;
		}
	}

	void play()
	{
		m_isPlaying = true;
	}
	void pause()
	{
		m_isPlaying = false;
	}
	void stop()
	{
		pause();
		m_currentFrame = 0;
		if (!m_repeat) m_finished = false;
	}
private:
	bool m_isPlaying = false, m_mirror = false, m_repeat = true, m_finished = false;
	float m_speed = 0, m_currentFrame = 0;
	std::vector<sf::IntRect> m_frames;
};


class AnimationManager : public MainLoopUpdater<Animation>
{
public:
	AnimationManager()
		: MainLoopUpdater<Animation>([&](AppInfo& app)
			{
				update(app);
			})
	{
	}
	AnimationManager(sf::Shape* target)
		: MainLoopUpdater<Animation>([&](AppInfo& app)
			{
				update(app);
			}), m_shape_target(target), m_isTargetSprite(false), m_valid(true)
	{
	}
	AnimationManager(sf::Sprite* target)
		: MainLoopUpdater<Animation>([&](AppInfo& app)
			{
				update(app);
			}), m_sprite_target(target), m_isTargetSprite(true), m_valid(true)
	{
	}

	void setTarget(sf::Shape* target)
	{
		if (target == nullptr)
		{
			m_valid = false;
			m_shape_target = nullptr;
			return;
		}

		m_valid = true;
		m_shape_target = target;
		m_isTargetSprite = false;
		m_sprite_target = nullptr;
	}
	void setTarget(sf::Sprite* target)
	{
		if (target == nullptr)
		{
			m_valid = false;
			m_sprite_target = nullptr;
			return;
		}

		m_valid = true;
		m_sprite_target = target;
		m_isTargetSprite = true;
		m_shape_target = nullptr;
	}

	template<typename T>
	const T* getTarget()
	{
		if (m_isTargetSprite) return m_sprite_target;
		return m_shape_target;
	}

	const sf::Sprite* getSpriteTarget()
	{
		return m_sprite_target;
	}
	const sf::Shape* getShapeTarget()
	{
		return m_shape_target;
	}

	void playAnimation(Animation& animation, bool pause_previous = false)
	{
		if (m_current == &animation) return;

		if (m_current != nullptr)
		if (pause_previous)
			m_current->pause();
		else
			m_current->stop();

		m_current = &animation;
		m_current->play();
	}
	void stopAnimation()
	{
		if (m_current != nullptr) m_current->stop();
	}
	void pauseAnimation()
	{
		if (m_current != nullptr) m_current->pause();
	}
	Animation* getCurrentAnimation() const
	{
		return m_current;
	}

	void update(AppInfo& app)
	{
		for (auto animation : containables())
		{
			if (animation->isPlaying() && animation != m_current)
				animation->stop();

			animation->update(app);
		}

		if (m_current == nullptr || !m_valid) return;

		if (m_isTargetSprite)
			m_sprite_target->setTextureRect(m_current->getCurrentFrame());
		else
			m_shape_target->setTextureRect(m_current->getCurrentFrame());
	}
private:
	sf::Shape* m_shape_target = nullptr;
	sf::Sprite* m_sprite_target = nullptr;
	bool m_isTargetSprite = false, m_valid = false;

	Animation* m_current = nullptr;
};

end_basics_namespace(graphics)

#endif