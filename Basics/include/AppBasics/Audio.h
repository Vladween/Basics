#pragma once
#ifndef APP_BASICS_AUDIO_H
#define APP_BASICS_AUDIO_H

#if _DEBUG == 1
#pragma comment(lib, "sfml-audio-d.lib")
#else
#pragma comment(lib, "sfml-audio.lib")
#endif

#include "Config.h"
#include "SFML/Audio.hpp"

#endif