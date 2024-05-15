#pragma once
#ifndef OFFICE_BASICS_FUNCTION_H
#define OFFICE_BASICS_FUNCTION_H

#include "AppBasics/Config.h"

begin_basics_namespace(office)

template<typename T>
class ElementEvent
{
public:
	ElementEvent() = default;
	ElementEvent(ElementEvent<T>* other, std::function<T> whenExecuted = nullptr, bool executeOnce = true, bool needsExecution = true)
		: m_other(other), m_whenExecuted(whenExecuted), m_executeOnce(executeOnce), m_needsExecution(needsExecution)
	{
	}
	ElementEvent(const ElementEvent<T>& other)
	{
		m_executeOnce = other.m_executeOnce;
		m_whenExecuted = other.m_whenExecuted;
	}

	void bindToEvent(ElementEvent<T>& other)
	{
		m_other = &other;
	}
	const ElementEvent<T>* getBoundEvent() const
	{
		return m_other;
	}

	void whenExecuted(std::function<T> whenExecuted)
	{
		m_whenExecuted = whenExecuted;
	}
	const std::function<T>& whenExecuted() const
	{
		return m_whenExecuted;
	}

	operator bool() const
	{
		return !m_needsExecution;
	}
	void operator=(const ElementEvent& other)
	{
		m_executeOnce = other.m_executeOnce;
		m_whenExecuted = other.m_whenExecuted;
	}
	template<typename... Args>
	auto operator()(Args... args)
	{
		if ((m_needsExecution && m_executeOnce) || !m_executeOnce)
		{
			m_needsExecution = false;
			if (m_other != nullptr)
				m_other->m_needsExecution = true;

			if (m_whenExecuted == nullptr) return;

			return m_whenExecuted(args...);
		}
		return;
	}
protected:
	void executeOnce(bool exeucte = true)
	{
		m_executeOnce = exeucte;
	}
	const bool& executesOnce() const
	{
		return m_executeOnce;
	}
private:
	std::function<T> m_whenExecuted = nullptr;
	ElementEvent<T>* m_other = nullptr;
	bool m_needsExecution = true, m_executeOnce = true;
};

template<typename T>
class RepeatedElementEvent : public ElementEvent<T>
{
public:
	using ElementEvent<T>::ElementEvent;
	using ElementEvent<T>::executeOnce;
	using ElementEvent<T>::executesOnce;
};

end_basics_namespace(office)

#endif