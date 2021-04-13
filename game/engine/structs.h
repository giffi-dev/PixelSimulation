#pragma once

#ifdef __cplusplus
extern "C" {
#endif 

typedef struct Vec2i {
    int x, y;
} Vec2i;

typedef struct Vec2f {
    float x, y;
} Vec2f;

#ifdef __cplusplus
}
#endif

#ifndef __cplusplus

#define RED     (SDL_Color){255, 90, 90, 255 }
#define BLUE    (SDL_Color){90,  90, 255,255 }
#define GREEN   (SDL_Color){90, 255, 90, 255 }
#define WHITE   (SDL_Color){217, 217, 217, 255 }
#define BLACK   (SDL_Color){18, 18, 18, 255 }
#define GRAY    (SDL_Color){90, 90, 90, 255 }
#define BROWN   (SDL_Color){106, 43, 0, 255 }
#define YELLOW   (SDL_Color){202, 192, 0, 255 }

#define bool _Bool
#define false 0
#define true  1

#else

#define RED     SDL_Color{255, 90, 90, 255 }
#define BLUE    SDL_Color{90,  90, 255,255 }
#define GREEN   SDL_Color{90, 255, 90, 255 }
#define WHITE   SDL_Color{217, 217, 217, 255 }
#define BLACK   SDL_Color{18, 18, 18, 255 }
#define GRAY    SDL_Color{90, 90, 90, 255 }
#define BROWN   SDL_Color{106, 43, 0, 255 }
#define YELLOW  SDL_Color{202, 192, 0, 255 }

#endif