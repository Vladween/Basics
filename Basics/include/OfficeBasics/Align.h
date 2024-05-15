#pragma once
#ifndef OFFICE_BASICS_ALIGN_H
#define OFFICE_BASICS_ALIGN_H

#include "AppBasics/Config.h"

begin_basics_namespace(office)

class Align
{
private:
	template<typename T>
	class AlignValue
	{
		friend class Align;
	public:
		AlignValue() = default;

		AlignValue(const T& value)
			: m_align_value(value)
		{
		}
		const T& operator=(const T& other)
		{
			m_align_value = other;
			m_value = 0;
			return m_align_value;
		}
		operator T& ()
		{
			return m_align_value;
		}
		operator const T& () const
		{
			return m_align_value;
		}

		AlignValue(const float& value)
			: m_value(value)
		{
		}
		const float& operator=(const float& other)
		{
			m_value = other;
			m_align_value = (T)0;
			return m_value;
		}
		operator float& ()
		{
			return m_value;
		}
		operator const float& () const
		{
			return m_value;
		}
	private:
		float m_value = 0;
		T m_align_value = (T)0;

		void resolve(float& changed_pos, float changed_size, float relative_pos, float relative_size) const
		{
			if (m_align_value == 0)
			{
				changed_pos = relative_pos + m_value;
				return;
			}

			switch (m_align_value)
			{
			case 1:
				changed_pos = relative_pos;
				break;
			case 2:
				changed_pos = relative_pos + relative_size / 2 - changed_size / 2;
				break;
			case 3:
				changed_pos = relative_pos + relative_size - changed_size;
				break;
			}
		}
	};
public:
	enum X { NoneX, Left, CenterX, Right };
	enum Y { NoneY, Top, CenterY, Bottom };
private:
	using AlignX = AlignValue<X>;
	using AlignY = AlignValue<Y>;
public:
	AlignX x;
	AlignY y;

	Align() = default;
	Align(const AlignX& _x, const AlignY& _y)
		: x(_x), y(_y)
	{
	}
	Align(const sf::Vector2f& align)
		: x(align.x), y(align.y)
	{
	}
	void operator=(const sf::Vector2f align)
	{
		x = align.x;
		y = align.y;
	}

	void resolve(sf::Text& text, sf::FloatRect bounds)
	{
		sf::Vector2f new_pos;
		x.resolve(new_pos.x, text.getGlobalBounds().width, bounds.left, bounds.width);
		y.resolve(new_pos.y, text.getGlobalBounds().height, bounds.top, bounds.height);
		text.setPosition(new_pos);
	}

	static const Align LeftTop, LeftCenter, LeftBottom, CenterTop, Middle, CenterBottom, RightTop, RightCenter, RightBottom;
};

const Align Align::LeftTop = Align(Align::Left, Align::Top);
const Align Align::LeftCenter = Align(Align::Left, Align::CenterY);
const Align Align::LeftBottom = Align(Align::Left, Align::Bottom);
const Align Align::CenterTop = Align(Align::CenterX, Align::Top);
const Align Align::Middle = Align(Align::CenterX, Align::CenterY);
const Align Align::CenterBottom = Align(Align::CenterX, Align::Bottom);
const Align Align::RightTop = Align(Align::Right, Align::Top);
const Align Align::RightCenter = Align(Align::Right, Align::CenterY);
const Align Align::RightBottom = Align(Align::Right, Align::Bottom);

end_basics_namespace(office)

#endif