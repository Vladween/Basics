#pragma once
#ifndef APP_BASICS_UPDATABLE_H
#define APP_BASICS_UPDATABLE_H

#include "Config.h"
#include "SFML/System.hpp"
#include <signal.h>

/// <summary>
/// Checks if the pointer points to a valid data
/// </summary>
/// <param name="ptr"> - a pointer for checking</param>
/// <returns>'true' if the pointer is valid, otherwise 'false'</returns>
bool is_valid(void* ptr)
{
	typedef void (*SignalHandlerPointer)(int);
	SignalHandlerPointer previousHandler;
	previousHandler = signal(SIGSEGV, [](int signal) { throw "!Access Violation!"; });

	try
	{
		*(int*)ptr = *(int*)ptr;
	}
	catch (const char*)
	{
		return false;
	}
	return true;
}

begin_basics_namespace(app)

std::ostream containables_stream(NULL);


template<class ContainableT, typename... ProcessArgs>
class Containable;

template<class ContainableT, typename... ProcessArgs>
class Container : public sf::NonCopyable
{
	using ContainerT = Container<ContainableT, ProcessArgs...>;
	using ContainableBaseT = Containable<ContainableT, ProcessArgs...>;
	friend class ContainableBaseT;
	static_assert(std::is_base_of<ContainableBaseT, ContainableT>::value, "Class 'ContainableT' must be derived from class 'Containable<ContainableT, ProcessArgs...>'!");
public:
	Container(std::function<void(ProcessArgs...)> process = nullptr)
	{
		if (process != nullptr && is_valid(&process)) m_processContainables = process;

		all_containers.push_back(this);

		containables_stream << "Container " << this << " created (type: " << typeid(*this).name() << ")\n";
		containables_stream << "All containers:\n";
		for (auto container : all_containers)
			containables_stream << "- " << container << "\n";
	}

	const bool& initialized() const
	{
		return m_initialized;
	}

	virtual ~Container()
	{
		if (m_initialized) return;

		auto itr = std::find(all_containers.begin(), all_containers.end(), this);
		if (itr == all_containers.end()) return;

		all_containers.erase(itr);
	}

	static void BindOnCreate(bool bind = true)
	{
		_BindOnCreate = bind;
	}
	static const bool& BindsOnCreate()
	{
		return _BindOnCreate;
	}

	static std::vector<const ContainerT*> AllContainers()
	{
		std::vector<const ContainerT*> res;
		for (auto container : all_containers)
			res.push_back(dynamic_cast<const ContainerT*>(container));
		return res;
	}

	static bool Push(ContainableBaseT* containable)
	{
		if (!is_valid(containable) || !_BindOnCreate || all_containers.empty()) return false;

		all_containers.back()->m_containables.push_back((ContainableT*)containable);
		containable->m_currentContainer = all_containers.back();

		return true;
	}
	static void Erase(ContainableBaseT* containable)
	{
		if (!is_valid(containable) || !is_valid(containable->m_currentContainer)) return;

		std::vector<ContainableT*>& containables = containable->m_currentContainer->m_containables;

		auto itr = std::find(containables.begin(), containables.end(), containable);
		if (itr == containables.end()) return;

		containables.erase(itr);
	}
protected:
	void startInit()
	{
		if (!m_initialized) return;

		auto itr = std::find(all_containers.begin(), all_containers.end(), this);
		if (itr != all_containers.end()) return;

		all_containers.push_back(this);

		m_initialized = false;
	}
	void endInit()
	{
		if (m_initialized) return;

		auto itr = std::find(all_containers.begin(), all_containers.end(), this);
		if (itr == all_containers.end()) return;

		all_containers.erase(itr);

		m_initialized = true;
	}

	void processContainables(ProcessArgs... args) const
	{
		m_processContainables(args...);
	}
	void processContainable(ContainableT* containable, ProcessArgs... args) const
	{
		if (!is_valid(containable) || !is_valid(&containable->Containable<ContainableT, ProcessArgs...>::m_process)) 
		{
			return;
		}

		containable->Containable<ContainableT, ProcessArgs...>::m_process(args...);
	}
	const std::vector<ContainableT*>& containables() const
	{
		return m_containables;
	}

	static void Bind(ContainableBaseT* containable, ContainerT* container)
	{
		if (!is_valid(containable) || !is_valid(container) || containable->m_currentContainer == container) return;

		Erase(containable);

		container->m_containables.push_back((ContainableT*)containable);
		containable->m_currentContainer = container;
	}
private:
	std::vector<ContainableT*> m_containables;
	std::function<void(ProcessArgs...)> m_processContainables = [&](ProcessArgs... args)
		{
			for (ContainableT* containable : containables())
				processContainable(containable, args...);
		};
	bool m_initialized = false;

	static std::vector<ContainerT*> all_containers;
	static bool _BindOnCreate;
};

template<class T, typename... Args>
std::vector<Container<T, Args...>*> Container<T, Args...>::all_containers = std::vector<Container<T, Args...>*>();
template<class T, typename... Args>
bool Container<T, Args...>::_BindOnCreate = true;

template<class ContainableT, typename... ProcessArgs>
class Containable
{
	using ContainerT = Container<ContainableT, ProcessArgs...>;
	friend class ContainerT;
public:
	Containable()
	{
		default_init([](ProcessArgs...) {});
	}
	Containable(std::function<void(ProcessArgs...)> process)
	{
		default_init(process);
	}

	Containable(const Containable<ContainableT, ProcessArgs...>& other) {}
	const Containable<ContainableT, ProcessArgs...>& operator=(const Containable<ContainableT, ProcessArgs...>& other) { return *this; }

	void bindToContainer(ContainerT& container)
	{
		ContainerT::Bind(this, &container);
	}
	const ContainerT* getCurrentContainer() const
	{
		return m_currentContainer;
	}

	virtual ~Containable()
	{
		ContainerT::Erase(this);
	}
private:
	std::function<void(ProcessArgs...)> m_process;
	ContainerT* m_currentContainer = nullptr;

	void default_init(std::function<void(ProcessArgs...)> process)
	{
		m_process = process;

		containables_stream << "Containable " << this << " of type " << typeid(*this).name() << " created\n";
		if (ContainerT::Push(this))
			containables_stream << "Containable " << this << " added successfully!\n";
		else
			containables_stream << "Containable " << this << " failed to add\n";

		containables_stream << "All containers of type " << typeid(ContainerT).name() << ":\n";
		for (auto container : ContainerT::all_containers)
			containables_stream << "- " << container << "\n";
	}
};

end_basics_namespace(app)

#endif