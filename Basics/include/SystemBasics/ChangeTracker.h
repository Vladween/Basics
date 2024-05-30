#pragma once
#ifndef SYSTEM_BASICS_CHANGE_TRACKER_H
#define SYSTEM_BASICS_CHANGE_TRACKER_H

#include "ConditionClasses.h"
#include "valid_ptr.h"

begin_basics_namespace(system)

template<typename T, bool>
class ChangeTracker;

template<typename T>
class ChangeTrackerBase
{
public:
	virtual bool changed() const = 0;

	const T& default_value() const
	{
		return m_default_value;
	}
protected:
	const T& default_value(const T& value)
	{
		m_default_value = value;
		return m_default_value;
	}
private:
	T m_default_value = T();
};

template<typename T, bool = is_class<T>::value>
class ChangeTracker : public ChangeTrackerBase<T>
{
public:
	ChangeTracker() = default;
	ChangeTracker(const T& other)
	{
		m_value = other;
		ChangeTrackerBase<T>::default_value(other);
	}

	const T& operator=(const T& other)
	{
		m_value = other;
		return m_value;
	}
	operator T& ()
	{
		return m_value;
	}
	operator const T& () const
	{
		return m_value;
	}

	bool changed() const override final
	{
		return m_value != ChangeTrackerBase<T>::default_value();
	}
private:
	T m_value = T();
};

template<typename T>
class ChangeTracker<T, true> : public ChangeTrackerBase<T>, public T
{
public:
	ChangeTracker() = default;
	ChangeTracker(const T& other)
		: T(other)
	{
		ChangeTrackerBase<T>::default_value(other);
	}

	const T& operator=(const T& other)
	{
		(T&)(*this) = other;
		return (T&)(*this);
	}

	bool changed() const override final
	{
		return (T&)(*this) != ChangeTrackerBase<T>::default_value();
	}
};

template<typename T, bool>
class PtrChangeTracker;

template<typename T>
class PtrChangeTrackerBase : public ChangeTrackerBase<T*>
{
	friend class PtrChangeTracker<T, true>;
	friend class PtrChangeTracker<T, false>;
public:
	PtrChangeTrackerBase() = default;
	PtrChangeTrackerBase(const T* const other)
	{
		m_value = (T*)other;
		ChangeTrackerBase<T*>::default_value((T*)other);
	}

	T& operator*() const
	{
		try
		{
			return *valid_ptr<T>(m_value);
		}
		rethrow(invalid_data_error)
	}
	operator T* () const
	{
		return m_value;
	}

	bool changed() const override final
	{
		return m_value == ChangeTrackerBase<T*>::default_value();
	}
private:
	T* m_value = NULL;
};

template<typename T, bool = is_class<T>::value>
class PtrChangeTracker : public PtrChangeTrackerBase<T>
{
public:
	using PtrChangeTrackerBase<T>::PtrChangeTrackerBase;
};

template<typename T>
class PtrChangeTracker<T, true> : public PtrChangeTrackerBase<T>
{
public:
	using PtrChangeTrackerBase<T>::PtrChangeTrackerBase;

	T* operator->() const
	{
		try
		{
			T* ptr = valid_ptr<T>(PtrChangeTrackerBase<T>::m_value);
			return ptr;
		}
		rethrow(invalid_data_error)
	}
};

template<typename T, bool is_T_class = is_class<T>::value>
class MutableChangeTracker : public ChangeTracker<T, is_T_class>
{
public:
	using ChangeTracker<T, is_T_class>::ChangeTracker;
	using ChangeTracker<T, is_T_class>::default_value;
};

template<typename T, bool is_T_class = is_class<T>::value>
class MutablePtrChangeTracker : public PtrChangeTracker<T, is_T_class>
{
public:
	using PtrChangeTracker<T, is_T_class>::PtrChangeTracker;
	using PtrChangeTracker<T, is_T_class>::default_value;
};

end_basics_namespace(system)

#endif // !SYSTEM_BASICS_CHANGE_TRACKER_H
