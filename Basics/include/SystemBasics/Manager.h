#pragma once
#ifndef SYSTEM_BASICS_MANAGER_H
#define SYSTEM_BASICS_MANAGER_H

#include "SystemBasics/Container.h"

begin_basics_namespace(system)

template<class ManagableT, typename... ProcessArgs>
class Managable;


template<class ManagableT, typename... ProcessArgs>
class Manager : public Container<ManagableT, ProcessArgs...>
{
	static_assert(std::is_base_of<Managable<ManagableT, ProcessArgs...>, ManagableT>::value, "Class 'ManagableT' must be derived from 'Managable<ManagableT, ProcessArgs...>'!");
	friend class Managable<ManagableT, ProcessArgs...>;
public:
	Manager(std::function<void(ProcessArgs...)> process = nullptr,
		std::function<void(ManagableT*)>    onAdd = nullptr,
		std::function<void(ManagableT*)>    onRemove = nullptr,
		std::function<void(ManagableT*)>    onCurrentChanged = nullptr)
		: Container<ManagableT, ProcessArgs...>(
			(process == nullptr)  ? _defaultManagerProcessFunction()  : process,
			(onAdd == nullptr)    ? _defaultManagerOnAddFunction()    : onAdd,
			(onRemove == nullptr) ? _defaultManagerOnRemoveFunction() : onRemove)
	{
		if (onCurrentChanged != nullptr) m_currentChanged = onCurrentChanged;
	}

	void setNextAsCurrent()
	{
		if (Container<ManagableT, ProcessArgs...>::containables().size() < 2) return;
		if (!is_valid(m_current))
		{
			m_current = Container<ManagableT, ProcessArgs...>::containables()[0];
			return;
		}

		int current_index = find(*m_current);
		if (current_index == -1) return;

		if (current_index + 1 >= Container<ManagableT, ProcessArgs...>::containables().size())
			m_current = Container<ManagableT, ProcessArgs...>::containables()[0];
		else
			m_current = Container<ManagableT, ProcessArgs...>::containables()[current_index + 1];
	}
	void setPreviousAsCurrent()
	{
		if (Container<ManagableT, ProcessArgs...>::containables().size() < 2) return;
		if (!is_valid(m_current))
		{
			m_current = Container<ManagableT, ProcessArgs...>::containables()[0];
			return;
		}

		int current_index = find(*m_current);
		if (current_index == -1) return;

		if (current_index == 0)
			m_current = Container<ManagableT, ProcessArgs...>::containables()[Container<ManagableT, ProcessArgs...>::containables().size() - 1];
		else
			m_current = Container<ManagableT, ProcessArgs...>::containables()[current_index - 1];
	}
	int find(ManagableT& managable) const
	{
		auto itr = std::find(Container<ManagableT, ProcessArgs...>::containables().begin(), Container<ManagableT, ProcessArgs...>::containables().end(), &managable);
		if (itr == Container<ManagableT, ProcessArgs...>::containables().end()) return -1;

		return itr - Container<ManagableT, ProcessArgs...>::containables().begin();
	}

	const ManagableT* getCurrentManagable() const
	{
		return m_current;
	}
protected:
	std::function<void(ProcessArgs...)> _defaultManagerProcessFunction() const
	{
		return [&](ProcessArgs... args)
			{
				if (is_valid(m_current))
					this->Container<ManagableT, ProcessArgs...>::processContainable(m_current, args...);
			};
	}
	std::function<void(ManagableT*)> _defaultManagerOnAddFunction()
	{
		return [&](ManagableT* managable)
			{
				if (Container<ManagableT, ProcessArgs...>::containables().empty())
					m_current = managable;
			};
	}
	std::function<void(ManagableT*)> _defaultManagerOnRemoveFunction()
	{
		return [&](ManagableT* managable)
			{
				if (Container<ManagableT, ProcessArgs...>::containables().size() == 1)
					m_current = nullptr;
				else if (managable == m_current)
				{
					auto itr = std::find(Container<ManagableT, ProcessArgs...>::containables().begin(), Container<ManagableT, ProcessArgs...>::containables().end(), managable);
					size_t index = itr - Container<ManagableT, ProcessArgs...>::containables().begin();

					if (index + 1 >= Container<ManagableT, ProcessArgs...>::containables().size())
						m_current = Container<ManagableT, ProcessArgs...>::containables()[0];
					else
						m_current = Container<ManagableT, ProcessArgs...>::containables()[index + 1];
				}
			};
	}
private:
	ManagableT* m_current = NULL;
	std::function<void(ManagableT*)> m_currentChanged = [](ManagableT*) {};
};


template<class ManagableT, typename... ProcessArgs>
class Managable : public Containable<ManagableT, ProcessArgs...>
{
	using ManagerT = Manager<ManagableT, ProcessArgs...>;
public:
	using Containable<ManagableT, ProcessArgs...>::Containable;

	void setAsCurrent() const
	{
		ManagerT* manager = (ManagerT*)Containable<ManagableT, ProcessArgs...>::_currentContainer();

		if (!is_valid(manager) || manager->m_current == (ManagableT*)this) return;

		manager->m_currentChanged((ManagableT*)this);
		manager->m_current = (ManagableT*)this;
	}
};

end_basics_namespace(system)

#endif // !SYSTEM_BASICS_MANAGER_H
