#pragma once
#ifndef SYSTEM_BASICS_IS_CLASS_H
#define SYSTEM_BASICS_IS_CLASS_H

#include "SystemBasics/Config.h"

begin_basics_namespace(system)

template<bool condition, typename TrueType, typename FalseType>
struct BASICS_API type_if { using value = TrueType; };
template<typename TrueType, typename FalseType>
struct BASICS_API type_if<false, TrueType, FalseType> { using value = FalseType; };


template<typename T>
struct BASICS_API is_class
{
	template<typename C> static char test(char C::* p) {}
	template<typename C> static int test(...) {}
	static const bool value = sizeof(is_class<T>::template test<T>(0)) == 1;
};


template<typename T>
struct BASICS_API is_pointer { static const bool value = false; };
template<typename T>
struct BASICS_API is_pointer<T*> { static const bool value = true; };

end_basics_namespace(system)

#endif // !SYSTEM_BASICS_IS_CLASS_H
