#pragma once
#ifndef GRAPHICS_BASICS_USER_INTERFACE_H
#define GRAPHICS_BASICS_USER_INTERFACE_H

#include "AppBasics/AppUpdatables.h"

namespace basics
{
	namespace graphics
	{
		class UserInterface : public sf::Drawable, public sf::View, public Updatable
		{
		public:
			sf::ContextSettings settings;
			sf::FloatRect globalBounds;
			sf::FloatRect moveBounds;
			float moveRatio = 1;


			void bindToFollow(sf::FloatRect& bound)
			{
				m_followedBounds = &bound;
			}
			const sf::FloatRect* getFollowedBoundsPtr() const
			{
				return m_followedBounds;
			}

			const sf::RenderTexture& getRenderTexture() const
			{
				return m_texture;
			}

			void draw(sf::Drawable& drawable)
			{
				m_drawables.push_back(&drawable);
			}
		protected:
			void update(AppInfo& app) override
			{
				if (m_followedBounds != nullptr)
				{
					if (moveBounds.width < m_followedBounds->width)
						moveBounds.width = m_followedBounds->width;
					if (moveBounds.height < m_followedBounds->height)
						moveBounds.height = m_followedBounds->height;

					if (moveBounds.left > m_followedBounds->left)
						moveBounds.left = m_followedBounds->left;
					if (moveBounds.left + moveBounds.width < m_followedBounds->left + m_followedBounds->width)
						moveBounds.left = m_followedBounds->left + m_followedBounds->width - moveBounds.width;

					if (moveBounds.top > m_followedBounds->top)
						moveBounds.top = m_followedBounds->top;
					if (moveBounds.top + moveBounds.height < m_followedBounds->top + m_followedBounds->height)
						moveBounds.top = m_followedBounds->top + m_followedBounds->height - moveBounds.height;
				}

				if (moveBounds != sf::FloatRect())
				{
					if (moveRatio > 1) moveRatio = 1;
					else if (moveRatio < 0) moveRatio = 0;

					if (moveRatio == 1)
					{
						setCenter(moveBounds.getPosition() + moveBounds.getSize() / 2.f);
					}
					else
					{
						move((moveBounds.getPosition() + moveBounds.getSize() / 2.f - getCenter()) * moveRatio * app.deltaTime);
					}					
				}

				if (globalBounds != sf::FloatRect())
				{
					if (getCenter().x - getSize().x / 2 < globalBounds.left)
						setCenter(globalBounds.left + getSize().x / 2, getCenter().y);
					else if (getCenter().x + getSize().x / 2 > globalBounds.left + globalBounds.width)
						setCenter(globalBounds.left + globalBounds.width - getSize().x / 2, getCenter().y);

					if (getCenter().y - getSize().y / 2 < globalBounds.top)
						setCenter(getCenter().x, globalBounds.top + getSize().y / 2);
					else if (getCenter().y + getSize().y / 2 > globalBounds.top + globalBounds.height)
						setCenter(getCenter().x, globalBounds.top + globalBounds.height - getSize().y / 2);
				}

				m_texture.create((uint32_t)getSize().x, (uint32_t)getSize().y, settings);
				m_texture.clear(sf::Color::Transparent);
				for (auto drawable : m_drawables)
					m_texture.draw(*drawable);
				m_texture.display();
				m_drawables.clear();
			}

			void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override
			{
				sf::RectangleShape rect(getSize());
				rect.setPosition(getCenter() - getSize() / 2.f);
				rect.setTexture(&m_texture.getTexture());
				target.draw(rect, states);
			}
		private:
			sf::RenderTexture m_texture;
			std::vector<sf::Drawable*> m_drawables;
			sf::FloatRect* m_followedBounds = nullptr;
		};
	}

	using namespace graphics;
}

#endif