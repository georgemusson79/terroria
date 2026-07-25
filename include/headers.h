// headers.h
#pragma once

// Workaround for MinGW GCC debug macro issues
#ifdef _GLIBCXX_DEBUG
#ifndef __glibcxx_requires_string
#define __glibcxx_requires_string(x)
#endif
#ifndef __glibcxx_requires_string_len
#define __glibcxx_requires_string_len(x, y)
#endif
#ifndef __glibcxx_requires_can_increment_range
#define __glibcxx_requires_can_increment_range(x, y, z)
#endif
#endif

// #include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <map>
#include <cassert>
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#undef min
#undef max