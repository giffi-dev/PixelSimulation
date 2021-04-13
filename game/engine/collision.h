#pragma once
#include "base.h"

#ifdef __cplusplus
extern "C" {
#endif

bool RectRect(SDL_Rect rect1, SDL_Rect rect2);
bool RectPoint(SDL_Rect rect, Vec2i point);

#ifdef __cplusplus
}
#endif