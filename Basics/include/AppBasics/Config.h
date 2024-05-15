#pragma once
#ifndef APP_BASICS_CONFIG_H
#define APP_BASICS_CONFIG_H

#ifdef BASICS_EXPORTS
#define BASICS_API __declspec(dllexport)
#else
#define BASICS_API __declspec(dllimport)
#endif

#if _DEBUG == 1
#pragma comment(lib, "sfml-system-d.lib")
#else
#pragma comment(lib, "sfml-system.lib")
#endif

#include "SFML/System.hpp"
#include <vector>
#include <thread>
#include <functional>
#include <type_traits>
#include <iostream>

#define MAIN(Type) int main() { Type app; app.run(); return 0; }
#define RUN(Type) Type app; app.run()
#define rethrow(exception_type) catch(exception_type ex) { throw ex; }

#define begin_basics_namespace(name) namespace basics { namespace name {
#define end_basics_namespace(name) } using namespace name;}

#endif // !APP_BASICS_CONFIG_H
