#pragma once
#ifndef OFFICE_BASICS_CLICKABLE_H
#define OFFICE_BASICS_CLICKABLE_H

#include "SystemBasics/Config.h"

begin_basics_namespace(office)

template<typename T>
class Clickable
{
public:
	bool clicked(bool when, T what)
	{
		if (!when && m_buttons[what])
		{
			m_buttons[what] = false;
			return true;
		}

		m_buttons[what] = true;
		return false;
	}
private:
	std::map<T, bool> m_buttons;
};

end_basics_namespace(office)

#endif