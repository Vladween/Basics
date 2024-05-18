#pragma once
#ifndef APP_BASICS_GRAPHICS_H
#define APP_BASICS_GRAPHICS_H

#include "SystemBasics/Config.h"

#if _DEBUG == 1
#pragma comment(lib, "sfml-window-d.lib")
#pragma comment(lib, "sfml-graphics-d.lib")
#else
#pragma comment(lib, "sfml-window.lib")
#pragma comment(lib, "sfml-graphics.lib")
#endif

#include "SFML/Graphics.hpp"

#endif