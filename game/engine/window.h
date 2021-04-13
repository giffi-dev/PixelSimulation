#pragma once
#include "base.h"

#ifdef __cplusplus
extern "C" {
#endif 

void CreateWindow(const char* title, int width, int height, SDL_WindowFlags flags);
void DestroyWindow();

// Place holder events, just to make the window work, make own sdl event loop later if needed
void DoEvents();
void RenderScreen();
void ClearScreen(SDL_Color color);
bool ShouldWindowClose();

// Setters & Getters //
void ToggleFullscreen();
void SetFullscreen(bool state);
bool GetFullscreen();

void  SetWindowSize(Vec2i size);
Vec2i GetWindowSize();

void SetTargetFPS(int fps);
int  GetTargetFPS();

#ifdef __cplusplus
}
#endif 
