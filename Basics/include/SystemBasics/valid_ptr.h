#pragma once
#ifndef SYSTEM_BASICS_VALID_PTR_H
#define SYSTEM_BASICS_VALID_PTR_H

#include "SystemBasics/Config.h"
#include <signal.h>
#include <sstream>

begin_basics_namespace(system)

bool is_valid(void* ptr)
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

bool is_valid(const void* ptr)
{
    return is_valid((void*)ptr);
}

class invalid_data_error : public std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
};

template<typename T>
class valid_ptr
{
public:
    valid_ptr() = default;
    valid_ptr(T* ptr)
    {
        try
        {
            _set(ptr);
        }
        rethrow(invalid_data_error)
    }
    valid_ptr(const T* ptr)
    {
        try
        {
            _set((T*)ptr);
        }
        rethrow(invalid_data_error)
    }
    T* operator=(T* other)
    {
        try
        {
            _set(other);
        }
        rethrow(invalid_data_error)
    }
    const T* operator=(const T* other)
    {
        try
        {
            _set((T*)other);
        }
        rethrow(invalid_data_error)
    }

    operator T* ()
    {
        try
        {
            return _get();
        }
        rethrow(invalid_data_error)
    }
    operator const T* () const
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
        if (!m_valid) throw _error();

        return m_ptr;
    }
    void _set(T* data)
    {
        if (!is_valid(data)) throw _error();

        m_ptr = data;
        m_valid = true;
    }
    invalid_data_error _error() const
    {
        std::stringstream ex;
        ex << "Data " << m_ptr << " contained in pointer " << this << " is invalid!";
        return invalid_data_error(ex.str());
    }
private:
    bool m_valid = false;
    T* m_ptr = nullptr;
};

end_basics_namespace(system)

#endif