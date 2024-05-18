#pragma once
#ifndef SYSTEM_BASICS_CONFIG_H
#define SYSTEM_BASICS_CONFIG_H

#ifdef BASICS_EXPORTS
#define BASICS_API __declspec(dllexport)
#else
#define BASICS_API __declspec(dllimport)
#endif

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

#define begin_private_basics_namespace namespace basics { namespace priv {
#define end_private_basics_namespace } }

#endif // !APP_BASICS_CONFIG_H
