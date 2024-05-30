#pragma once
#ifndef SYSTEM_BASICS_CONTAINER_H
#define SYSTEM_BASICS_CONTAINER_H

#include "SFMLBasics/System.h"
#include "valid_ptr.h"

begin_basics_namespace(system)

std::ostream containers_stream(NULL);

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
	Container(std::function<void(ProcessArgs...)> process = nullptr, std::function<void(ContainableT*)> onContainableAdded = nullptr, std::function<void(ContainableT*)> onContainableRemoved = nullptr)
	{
		if (process != nullptr)
			m_processContainables = process;
		if (onContainableAdded != nullptr) 
			m_containableAdded = onContainableAdded;
		if (onContainableRemoved != nullptr) 
			m_containableRemoved = onContainableRemoved;

		all_containers.push_back(this);

		containers_stream << "Container " << this << " created (type: " << typeid(*this).name() << ")\n";
		containers_stream << "All containers:\n";
		for (auto container : all_containers)
			containers_stream << "- " << container << "\n";
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

		all_containers.back()->m_containableAdded((ContainableT*)containable);
		all_containers.back()->m_containables.push_back((ContainableT*)containable);		
		containable->m_onContainerChanged(all_containers.back());
		containable->m_currentContainer = all_containers.back();		

		return true;
	}
	static bool Erase(ContainableBaseT* containable)
	{
		if (!is_valid(containable) || !is_valid(containable->m_currentContainer)) return false;

		std::vector<ContainableT*>& containables = containable->m_currentContainer->m_containables;

		auto itr = std::find(containables.begin(), containables.end(), containable);
		if (itr == containables.end()) return false;

		containable->m_currentContainer->m_containableRemoved((ContainableT*)containable);
		containables.erase(itr);
		containable->m_currentContainer = nullptr;

		return true;
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

	std::function<void(ProcessArgs...)> defaultContainerProcessFunction() const
	{
		return [&](ProcessArgs... args)
			{
				for (ContainableT* containable : containables())
					processContainable(containable, args...);
			};
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

	static bool Bind(ContainableBaseT* containable, ContainerT* container)
	{
		if (!is_valid(containable) || !is_valid(container)) return false;
		if (containable->m_currentContainer == container) return true;

		Erase(containable);

		container->m_containableAdded((ContainableT*)containable);
		container->m_containables.push_back((ContainableT*)containable);		
		containable->m_onContainerChanged(container);
		containable->m_currentContainer = container;	

		return true;
	}
private:
	std::vector<ContainableT*> m_containables;
	std::function<void(ProcessArgs...)> m_processContainables = [&](ProcessArgs... args)
		{
			for (ContainableT* containable : containables())
				processContainable(containable, args...);
		};
	std::function<void(ContainableT*)> m_containableAdded = [](ContainableT*) {}, m_containableRemoved = [](ContainableT*) {};
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
		_default_init([](ProcessArgs...) {}, [](ContainerT*) {});
	}
	Containable(std::function<void(ProcessArgs...)> process, std::function<void(ContainerT*)> onContainerChanged = [](ContainerT*){})
	{
		_default_init(process, onContainerChanged);
	}

	Containable(const Containable<ContainableT, ProcessArgs...>& other) {}
	const Containable<ContainableT, ProcessArgs...>& operator=(const Containable<ContainableT, ProcessArgs...>& other) { return *this; }

	virtual ~Containable()
	{
		ContainerT::Erase(this);
	}

protected:
	void _currentContainer(ContainerT& container)
	{
		ContainerT::Bind(this, &container);
	}
	const ContainerT* _currentContainer() const
	{
		return m_currentContainer;
	}
private:
	std::function<void(ProcessArgs...)> m_process;
	std::function<void(ContainerT*)> m_onContainerChanged;
	ContainerT* m_currentContainer = nullptr;

	void _default_init(std::function<void(ProcessArgs...)> process, std::function<void(ContainerT*)> onContainerChanged)
	{
		m_process = process;
		m_onContainerChanged = onContainerChanged;

		containers_stream << "Containable " << this << " of type " << typeid(*this).name() << " created\n";
		if (ContainerT::Push(this))
			containers_stream << "Containable " << this << " added successfully!\n";
		else
			containers_stream << "Containable " << this << " failed to add\n";

		containers_stream << "All containers of type " << typeid(ContainerT).name() << ":\n";
		for (auto container : ContainerT::all_containers)
			containers_stream << "- " << container << "\n";
	}
};

end_basics_namespace(system)

#endif