#pragma once

#ifdef NO_TTF
#define SDL_TTF_H_
#endif


#include <stdio.h>

#if _MSC_VER >= 1920   // VS 2019
    #include <SDL.h>
    #include <SDL_image.h>
    #include <SDL_ttf.h>

    #define GIF_ASSERT(x, y) if(!x) {                        \
        printf("Assertion Failed: %s\n",y); \
        /*__debugbreak();*/ }
#elif defined(__unix__) // w/ g++ compiler
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_image.h>
    #include <SDL2/SDL_ttf.h>

    #define GIF_ASSERT(x, y) if(!x){ printf("Assertion Failed: %s\n",y);}
#elif defined(__SWITCH__)
    #include <SDL.h>
    #include <SDL_image.h>
    #include <SDL_ttf.h>
    #include <switch.h>
    #include <unistd.h>

    #define GIF_ASSERT(x, y) if(!x){}
#else
    #error Unsupported Editor/Platform, You re on your own buddy.
#endif

#include "structs.h"
