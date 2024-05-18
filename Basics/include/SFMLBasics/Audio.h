#pragma once
#ifndef SFML_BASICS_AUDIO_H
#define SFML_BASICS_AUDIO_H

#include "SystemBasics.h"

#if _DEBUG == 1
#pragma comment(lib, "sfml-audio-d.lib")
#else
#pragma comment(lib, "sfml-audio.lib")
#endif

#include "SFML/Audio.hpp"

#endif