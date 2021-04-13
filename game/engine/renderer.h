#pragma once
#include "base.h"

#ifdef __cplusplus
extern "C" {
#endif 

/*
Basic wrapper for sdl2 render functions,
without having to pass in the SDL_Renderer*
Also makes using different colors easier

Functions does not take pointers as arguments for color, rect
or other values, because i wanted to make this easy to use;
without having to initalize variables before passing them in.
This will result in few unnecessary copying.

E.G.
	void		DrawRect((SDL_Rect){0,0,16,16}, BLACK};
instead of
	SDL_Color	color = BLACK;
	SDL_Rect	rect = {0,0,16,16};
	void		DrawRect(&rect, &color);
*/

// Basic Shapes //

void DrawRect(SDL_Rect rect, SDL_Color color);
void DrawLine(Vec2i start_pos, Vec2i end_pos, SDL_Color color);
void DrawTriangle(Vec2i pos1, Vec2i pos2, Vec2i pos3, SDL_Color color);
void DrawPixel(Vec2i pos, SDL_Color color);

void DrawFillRect(SDL_Rect rect, SDL_Color color);

// Textures //

SDL_Texture* LoadTexture(const char* file_path);
void DestroyTexture(SDL_Texture* texture);

void DrawTexture(SDL_Texture* texture, Vec2i position);
void DrawTextureEx(SDL_Texture* texture, Vec2i position, bool vflip, bool hflip);
void DrawTexturePro(SDL_Texture* texture, SDL_Rect src, SDL_Rect dst, double Angle, SDL_Point center, bool vflip, bool hflip);

void DrawTextureFrame(SDL_Texture* texture, Vec2i pos, SDL_Rect frame);

// Font //
#ifndef NO_TTF

TTF_Font* OpenFont(const char* fpath, int psize);
void	  CloseFont(TTF_Font* font);
void	  DrawText(const char* text, TTF_Font* font, Vec2i position, SDL_Color color, bool centered);

#endif

// Utils //

void SetRenderColor(SDL_Color color);
void SetRenderScale(Vec2f scale);
void SetViewport(SDL_Rect rect);

SDL_Color GetRenderColor();
Vec2f     GetRenderScale();
SDL_Rect  GetViewport();
SDL_Renderer* GetRenderer();

#ifdef __cplusplus
}
#endif 