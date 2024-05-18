#pragma once
#ifndef GRAPHICS_BASICS_ANIMATION_H
#define GRAPHICS_BASICS_ANIMATION_H

#include "AppBasics/AppUpdatables.h"
#include "SystemBasics/valid_ptr.h"

begin_basics_namespace(graphics)

class AnimationManager;
class Animation;

end_basics_namespace(graphics)
begin_private_basics_namespace

void _set_as_current(Animation* animation, AnimationManager* manager);

end_private_basics_namespace
begin_basics_namespace(graphics)

class Animation : public Containable<Animation>, public Updatable
{
public:
	friend class AnimationManager;

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

	bool isFinished() const
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
		if (m_currentFrame >= m_frames.size()) return {};
		return m_frames[(size_t)m_currentFrame];
	}
	sf::IntRect getMirroredCurrentFrame() const
	{
		if (m_currentFrame >= m_frames.size()) return {};
		return sf::IntRect(m_frames[(size_t)m_currentFrame].left + m_frames[(size_t)m_currentFrame].width, m_frames[(size_t)m_currentFrame].top,
			-m_frames[(size_t)m_currentFrame].width, m_frames[(size_t)m_currentFrame].height);
	}
	size_t getCurrentFrameIndex() const
	{
		return (size_t)m_currentFrame;
	}

	void setTexture(const sf::Texture* texture)
	{
		m_texture = (sf::Texture*)texture;
	}
	const sf::Texture* getTexture() const
	{
		return m_texture;
	}

	void apply(sf::Shape& shape)
	{
		m_shape_target = &shape;
		m_sprite_target = NULL;
		_apply_frame();
	}
	void apply(sf::Sprite& sprite)
	{
		m_sprite_target = &sprite;
		m_shape_target = NULL;
		_apply_frame();
	}
	
	const sf::Sprite* getSpriteTarget() const
	{
		return m_sprite_target;
	}
	const sf::Shape* getShapeTarget() const
	{
		return m_shape_target;
	}

	void play()
	{
		m_isPlaying = true;

		if (!is_valid(Containable<Animation>::getCurrentContainer())) return;

		priv::_set_as_current(this, (AnimationManager*)Containable<Animation>::getCurrentContainer());
		_apply_frame();
	}
	void pause()
	{
		m_isPlaying = false;
	}
	void reset()
	{
		m_currentFrame = 0;
		if (!m_repeat) m_finished = false;
		_apply_frame();
	}
	void stop()
	{
		pause();
		reset();
	}	
protected:
	void _apply_frame()
	{
		bool texture_is_valid = is_valid(m_texture);
		if (is_valid(m_shape_target))
		{
			m_shape_target->setTextureRect(getCurrentFrame());

			if (texture_is_valid) m_shape_target->setTexture(m_texture);
		}
		if (is_valid(m_sprite_target))
		{
			m_sprite_target->setTextureRect(getCurrentFrame());

			if (texture_is_valid) m_sprite_target->setTexture(*m_texture);
		}
	}

	void update(AppInfo& app) override
	{
		if (!m_isPlaying) return;

		_apply_frame();

		m_currentFrame += m_speed * app.deltaTime;

		if ((size_t)m_currentFrame > m_frames.size() - 1)
		{
			if (m_repeat) m_currentFrame = 0;
			else 
			{
				m_finished = true;
				m_currentFrame = m_frames.size() - 1;
			}
		}
	}
private:
	bool m_isPlaying = false, m_mirror = false, m_repeat = true, m_finished = false;
	float m_speed = 0, m_currentFrame = 0;
	std::vector<sf::IntRect> m_frames;

	sf::Texture* m_texture = NULL;
	sf::Shape* m_shape_target = NULL;
	sf::Sprite* m_sprite_target = NULL;
};

class AnimationManager : public Container<Animation>
{
	friend class Animation;
	friend void priv::_set_as_current(Animation*, AnimationManager*);
public:
	AnimationManager() = default;
	AnimationManager(sf::Sprite& sprite)
		: m_sprite_target(&sprite)
	{
	}
	AnimationManager(sf::Shape& shape)
		: m_shape_target(&shape)
	{
	}

	using Container<Animation>::endInit;
	using Container<Animation>::startInit;

	void setTarget(sf::Sprite& sprite)
	{
		m_sprite_target = &sprite;
		m_shape_target = NULL;

		for (auto animation : Container<Animation>::containables())
			animation->apply(*m_sprite_target);
	}
	void setTarget(sf::Shape& shape)
	{
		m_shape_target = &shape;
		m_sprite_target = NULL;

		for (auto animation : Container<Animation>::containables())
			animation->apply(*m_shape_target);
	}
	const void* getTargetPtr()
	{
		if (is_valid(m_sprite_target)) return m_sprite_target;
		if (is_valid(m_shape_target)) return m_shape_target;
		return nullptr;
	}

	Animation* getCurrentAnimation() const
	{
		return m_current;
	}
private:
	Animation* m_current = NULL;

	sf::Texture* m_texutre = NULL;
	sf::Sprite* m_sprite_target = NULL;
	sf::Shape* m_shape_target = NULL;
};

end_basics_namespace(graphics)

begin_private_basics_namespace

void _set_as_current(Animation* animation, AnimationManager* manager)
{
	if (manager->m_current == animation) return;

	if (is_valid(manager->m_current)) manager->m_current->stop();

	manager->m_current = animation;
}

end_private_basics_namespace

#endif