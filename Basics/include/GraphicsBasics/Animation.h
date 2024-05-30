#pragma once
#ifndef GRAPHICS_BASICS_ANIMATION_H
#define GRAPHICS_BASICS_ANIMATION_H

#include "AppBasics/Updatable.h"
#include "SystemBasics/Manager.h"
#include "SystemBasics/ChangeTracker.h"
#include "SFMLBasics/Graphics.h"

begin_basics_namespace(graphics)

class Animation;
class AnimationManager;

template<
	typename bool_t = bool,
	typename float_t = float,
	typename texture_ptr_t = sf::Texture*,
	typename shape_ptr_t = sf::Shape*,
	typename sprite_ptr_t = sf::Sprite*
>
class AnimationTraits
{
	friend class Animation;
	friend class AnimationManager;
public:
	virtual void setMirrored(bool mirror = true) = 0;
	bool isMirrored() const
	{
		return m_mirror;
	}

	virtual void setRepeated(bool repeat = true) = 0;
	bool isRepeated() const
	{
		return m_repeat;
	}

	virtual void setSpeed(float speed) = 0;
	float getSpeed() const
	{
		return m_speed;
	}

	virtual void setMirrorFunction(const std::function<sf::IntRect(const sf::IntRect&)>& getMirroredFrame) = 0;
	const std::function<sf::IntRect(const sf::IntRect&)>& getMirrorFunction() const
	{
		return m_getMirroredFrame;
	}

	virtual void setTexture(const sf::Texture* texture) = 0;
	const sf::Texture* getTexture() const
	{
		return m_texture;
	}

	virtual void apply(sf::Shape& shape) = 0;
	virtual void apply(sf::Sprite& sprite) = 0;
	const sf::Shape* getShapeTarget() const
	{
		return m_shapeTarget;
	}
	const sf::Sprite* getSpriteTarget() const
	{
		return m_spriteTarget;
	}
private:
	bool_t m_mirror, m_repeat;
	float_t m_speed = 0;
	std::function<sf::IntRect(const sf::IntRect&)> m_getMirroredFrame = [](const sf::IntRect& r) { return sf::IntRect(r.left + r.width, r.top, -r.width, r.height); };

	texture_ptr_t m_texture = NULL;
	shape_ptr_t m_shapeTarget = NULL;
	sprite_ptr_t m_spriteTarget = NULL;
};

class Animation : public AnimationTraits<>, public Managable<Animation>, public Updatable
{
public:
	void setFrames(const std::vector<sf::IntRect>& frames)
	{
		m_frames = frames;

		if ((size_t)m_currentFrame >= m_frames.size())
			m_currentFrame = m_frames.size() - 1;
		_applyFrame();
	}
	const std::vector<sf::IntRect>& getFrames() const
	{
		return m_frames;
	}

	void setFrame(size_t index, const sf::IntRect& frame)
	{
		if (index >= m_frames.size()) return;

		m_frames[index] = frame;

		if (index == (size_t)m_currentFrame) _applyFrame();
	}
	sf::IntRect getNormalFrame(size_t index) const
	{
		if (index >= m_frames.size()) return {};

		return m_frames[index];
	}
	sf::IntRect getMirroredFrame(size_t index) const
	{
		if (index >= m_frames.size()) return {};

		return m_getMirroredFrame(m_frames[index]);
	}
	sf::IntRect getFrame(size_t index) const
	{
		if (m_mirror) return getMirroredFrame(index);
		return getNormalFrame(index);
	}

	void setCurrentFrame(size_t index)
	{
		if (index >= m_frames.size())
			index = m_frames.size() - 1;

		if ((size_t)m_currentFrame == m_frames.size() - 1 && index < m_frames.size() - 1)
			m_finished = false;
		if ((size_t)m_currentFrame <= m_frames.size() - 1 && index == m_frames.size() - 1 && !m_repeat)
			m_finished = true;

		m_currentFrame = index;
		_applyFrame();
	}
	void setCurrentFrame(const sf::IntRect& frame)
	{
		if ((size_t)m_currentFrame >= m_frames.size()) return;

		m_frames[(size_t)m_currentFrame] = frame;
	}
	sf::IntRect getNormalCurrentFrame() const
	{
		if ((size_t)m_currentFrame >= m_frames.size()) return {};

		return m_frames[(size_t)m_currentFrame];
	}
	sf::IntRect getMirroredCurrentFrame() const
	{
		if ((size_t)m_currentFrame >= m_frames.size()) return {};

		return m_getMirroredFrame(m_frames[(size_t)m_currentFrame]);
	}
	sf::IntRect getCurrentFrame() const
	{
		if (m_mirror) return getMirroredCurrentFrame();
		return getNormalCurrentFrame();
	}
	size_t getCurrentFrameIndex() const
	{
		return (size_t)m_currentFrame;
	}

	void setFinished(bool finished = true)
	{
		if (m_repeat) return;

		if (finished)
			end();
		else
			reset();

		m_finished = finished;
	}
	bool isFinished() const
	{
		return m_finished;
	}

	void play()
	{
		m_isPlaying = true;
		setAsCurrent();
	}
	void pause()
	{
		m_isPlaying = false;
	}
	void reset()
	{
		setCurrentFrame(0);
	}
	void end()
	{
		setCurrentFrame(m_frames.size() - 1);
	}
	void stop()
	{
		pause();
		reset();
	}
	const bool& isPlaying() const
	{
		return m_isPlaying;
	}

	void bindToManager(Manager<Animation>& manager)
	{
		Managable<Animation>::_currentContainer(manager);
	}
	void unbindFromManager()
	{
		Manager<Animation>::Erase(this);
	}
	const AnimationManager* getCurrentManager() const
	{
		return (const AnimationManager*)Managable<Animation>::_currentContainer();
	}
	using Managable<Animation>::_currentContainer;

	void setMirrored(bool mirror = true) override final
	{
		m_mirror = mirror;
		_applyFrame();
	}
	void setRepeated(bool repeat = true) override final
	{
		m_repeat = repeat;
	}
	void setSpeed(float speed) override final
	{
		m_speed = speed;
	}
	void setMirrorFunction(const std::function<sf::IntRect(const sf::IntRect&)>& getMirroredFrame) override final
	{
		m_getMirroredFrame = getMirroredFrame;
		_applyFrame();
	}
	void setTexture(const sf::Texture* texture) override final
	{
		m_texture = const_cast<sf::Texture*>(texture);
		_applyFrame();
	}
	void apply(sf::Sprite& sprite) override final
	{
		m_spriteTarget = &sprite;
		m_shapeTarget = NULL;
		_applyFrame();
	}
	void apply(sf::Shape& shape) override final
	{
		m_shapeTarget = &shape;
		m_spriteTarget = NULL;
		_applyFrame();
	}
protected:
	using Managable<Animation>::setAsCurrent;


	void _applyFrame()
	{
		if (is_valid(m_shapeTarget))
		{
			m_shapeTarget->setTextureRect(getCurrentFrame());

			if (is_valid(m_texture) && m_shapeTarget->getTexture() != m_texture)
				m_shapeTarget->setTexture(m_texture);
		}
		if (is_valid(m_spriteTarget))
		{
			m_spriteTarget->setTextureRect(getCurrentFrame());

			if (is_valid(m_texture) && m_spriteTarget->getTexture() != m_texture)
				m_spriteTarget->setTexture(*m_texture);
		}
	}

	void update(AppInfo& app) override
	{
		if (!m_isPlaying) return;

		_applyFrame();

		m_currentFrame += m_speed * app.deltaTime;

		if ((size_t)m_currentFrame > m_frames.size() - 1)
		{
			if (m_repeat) setCurrentFrame(0);
			else
			{
				setCurrentFrame(m_frames.size() - 1);
				m_finished = true;
			}
		}
	}
private:


	bool m_isPlaying = false, m_finished = false;
	float m_currentFrame = 0;
	std::vector<sf::IntRect> m_frames;
};


class AnimationManager : public AnimationTraits<
	ChangeTracker<bool>,
	ChangeTracker<float>,
	PtrChangeTracker<sf::Texture>,
	PtrChangeTracker<sf::Shape>,
	PtrChangeTracker<sf::Sprite>>, public Manager<Animation>
{
public:
	AnimationManager()
		: Manager<Animation>([]() {}, [&](Animation* anim)
			{
				anim->m_getMirroredFrame = m_getMirroredFrame;

				if (m_repeat.changed())
					anim->setRepeated(m_repeat);
				if (m_mirror.changed())
					anim->setMirrored(m_mirror);
				if (m_speed.changed())
					anim->setSpeed(m_speed);

				if (m_texture.changed())
					anim->setTexture((sf::Texture*)m_texture);
				if (m_shapeTarget.changed() && is_valid(m_shapeTarget))
					anim->apply(*(sf::Shape*)m_shapeTarget);
				if (m_spriteTarget.changed() && is_valid(m_spriteTarget))
					anim->apply(*(sf::Sprite*)m_spriteTarget);
			}, [](Animation*) {}, [](Animation*) {})
	{
	}

	const Animation* getCurrentAnimation() const
	{
		return getCurrentManagable();
	}

	void setMirrored(bool mirror = true) override final
	{
		m_mirror = mirror;
		for (auto animation : containables())
			animation->setMirrored(mirror);
	}
	void setRepeated(bool repeat = true) override final
	{
		m_repeat = repeat;
		for (auto animation : containables())
			animation->setRepeated(repeat);
	}
	void setSpeed(float speed) override final
	{
		m_speed = speed;
		for (auto animation : containables())
			animation->setSpeed(speed);
	}
	void setMirrorFunction(const std::function<sf::IntRect(const sf::IntRect&)>& getMirroredFrame) override final
	{
		m_getMirroredFrame = getMirroredFrame;
		for (auto animation : containables())
			animation->setMirrorFunction(getMirroredFrame);
	}
	void setTexture(const sf::Texture* texture) override final
	{
		m_texture = const_cast<sf::Texture*>(texture);
		for (auto animation : containables())
			animation->setTexture(texture);
	}
	void apply(sf::Sprite& sprite) override final
	{
		m_spriteTarget = &sprite;
		m_shapeTarget = NULL;
		for (auto animation : containables())
			animation->apply(sprite);
	}
	void apply(sf::Shape& shape) override final
	{
		m_shapeTarget = &shape;
		m_spriteTarget = NULL;
		for (auto animation : containables())
			animation->apply(shape);
	}
private:
	using Manager<Animation>::getCurrentManagable;
};

end_basics_namespace(graphics)

#endif