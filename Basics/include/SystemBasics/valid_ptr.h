#pragma once
#ifndef SYSTEM_BASICS_VALID_PTR_H
#define SYSTEM_BASICS_VALID_PTR_H

#include "SystemBasics/ConditionClasses.h"
#include <signal.h>
#include <sstream>

begin_basics_namespace(system)

inline BASICS_API bool is_valid(void* ptr)
{
    if (!ptr) return false;

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

inline BASICS_API bool is_valid(const void* ptr)
{
    return is_valid((void*)ptr);
}

class invalid_data_error : public std::exception
{
public:
    using std::exception::exception;
};

template<typename T>
class valid_ptr_base
{
public:
    valid_ptr_base() = default;
    valid_ptr_base(const T* const ptr)
    {
        try
        {
            _set((T*)ptr);
        }
        rethrow(invalid_data_error)
    }

    operator bool() const
    {
        return is_valid(m_ptr);
    }

    T& operator*() const
    {
        try
        {
            return *_get();
        }
        rethrow(invalid_data_error)
    }
    T* operator=(const T* const other)
    {
        try
        {
            _set((T*)other);
        }
        rethrow(invalid_data_error)
    }
    operator T* () const
    {
        try
        {
            return _get();
        }
        rethrow(invalid_data_error)
    }
protected:
    T* _get() const
    {
        if (!m_valid) throw _error(m_ptr);

        return m_ptr;
    }
    void _set(T* data)
    {
        if (!is_valid(data)) throw _error(data);

        m_ptr = data;
        m_valid = true;
    }
    invalid_data_error _error(const T* ptr) const
    {
        std::stringstream ex;
        ex << "Data " << ptr << " contained in pointer " << this << " is invalid!";
        return invalid_data_error(ex.str().c_str());
    }
private:
    bool m_valid = false;
    T* m_ptr = nullptr;
};

template<typename T, bool = is_class<T>::value>
class valid_ptr : public valid_ptr_base<T>
{
public:
    using valid_ptr_base<T>::valid_ptr_base;
};

template<typename T>
class valid_ptr<T, true> : public valid_ptr_base<T>
{
public:
    using valid_ptr_base<T>::valid_ptr_base;

    T* operator->() const
    {
        try
        {
            return valid_ptr_base<T>::_get();
        }
        rethrow(invalid_data_error)
    }
};

end_basics_namespace(system)

#endif