#include "renderer.h"
#include "window.h"
#include "fps_calc.h"

extern SDL_Renderer* renderer; // In window.c

// Basic Hollow Shapes //

void DrawRect(SDL_Rect rect, SDL_Color color) {
    SetRenderColor(color);
    SDL_RenderDrawRect(renderer, &rect);
}

void DrawLine(Vec2i sp, Vec2i ep, SDL_Color color) {
    SetRenderColor(color);
    SDL_RenderDrawLine(renderer, sp.x, sp.y, ep.x, ep.y);
}

void DrawTriangle(Vec2i p1, Vec2i p2, Vec2i p3, SDL_Color color) {
    SetRenderColor(color);
    SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y);
    SDL_RenderDrawLine(renderer, p2.x, p2.y, p3.x, p3.y);
    SDL_RenderDrawLine(renderer, p3.x, p3.y, p1.x, p1.y);
}

void DrawPixel(Vec2i pos, SDL_Color color) {
    SetRenderColor(color);
    SDL_Rect point = { pos.x, pos.y, 1, 1 };
    
    //SDL_RenderDrawPoint does not take in count renderscale, so im using fillrect instead.
    SDL_RenderFillRect(renderer, &point);
}

// Basic Fill Shapes //

void DrawFillRect(SDL_Rect rect, SDL_Color color) {
    SetRenderColor(color);
    SDL_RenderFillRect(renderer, &rect);
}

// Textures //

SDL_Texture* LoadTexture(const char* fpath) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, fpath);
    GIF_ASSERT(texture, SDL_GetError());
    return texture;
}

void DestroyTexture(SDL_Texture* texture) {
    SDL_DestroyTexture(texture);
}

void DrawTexture(SDL_Texture* texture, Vec2i pos) {
    SDL_Rect src = {0,0,0,0};
    SDL_QueryTexture(texture, NULL, NULL, &src.w, &src.h);

    SDL_Rect dst = src;
    dst.x = pos.x;
    dst.y = pos.y;
    
    SDL_RenderCopy(renderer, texture, &src, &dst);
}

void DrawTextureEx(SDL_Texture* texture, Vec2i pos, bool vflip, bool hflip) {
    SDL_Rect src = { 0,0,0,0 };
    SDL_QueryTexture(texture, NULL, NULL, &src.w, &src.h);

    SDL_Rect dst = src;
    dst.x = pos.x;
    dst.y = pos.y;

    DrawTexturePro(texture, src, dst, 0.0f, (SDL_Point) {0,0}, vflip, hflip);
}

void DrawTexturePro(SDL_Texture* texture, SDL_Rect src, SDL_Rect dst, double Angle, SDL_Point center, bool vflip, bool hflip) {
    int flip = SDL_FLIP_NONE;
    flip |= SDL_FLIP_VERTICAL   * (vflip * 1);
    flip |= SDL_FLIP_HORIZONTAL * (hflip * 1);

    SDL_RenderCopyEx(renderer, texture, &src, &dst, 0.0f, &center, (SDL_RendererFlip)flip);
}

void DrawTextureFrame(SDL_Texture* texture, Vec2i pos, SDL_Rect src) {
    SDL_Rect dst = src;
    dst.x = pos.x;
    dst.y = pos.y;

    SDL_RenderCopy(renderer, texture, &src, &dst);
}

// Font //
#ifndef NO_TTF
TTF_Font* OpenFont(const char* fpath, int psize)
{
    TTF_Font* font = TTF_OpenFont(fpath, psize);
    GIF_ASSERT(font, SDL_GetError());
    return font;
}

void CloseFont(TTF_Font* font)
{
    TTF_CloseFont(font);
}

void DrawText(const char* text, TTF_Font* font, Vec2i pos, SDL_Color color, bool centered)
{
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color); // Did you remember to open the Font file?
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect dst;
    SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
    dst.x = pos.x;
    dst.y = pos.y;
    if (centered)
    {
        dst.x -= (dst.w / 2);
        dst.y -= (dst.h / 2);
    }

    SDL_RenderCopy(renderer, texture, NULL, &dst);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}
#endif//NO_TTF

// Utils //

void SetRenderColor(SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void SetRenderScale(Vec2f scale) {
    SDL_RenderSetScale(renderer, scale.x, scale.y);
}

void SetViewport(SDL_Rect rect) {
    SDL_RenderSetViewport(renderer, &rect);
}

SDL_Color GetRenderColor() {
    SDL_Color color;
    SDL_GetRenderDrawColor(renderer, &color.r, &color.g, &color.b, &color.a);
    return color;
}

Vec2f GetRenderScale() {
    Vec2f scale;
    SDL_RenderGetScale(renderer, &scale.x, &scale.y);
    return scale;
}

SDL_Rect GetViewport() {
    SDL_Rect rect;
    SDL_RenderGetViewport(renderer, &rect);
    return rect;
}

SDL_Renderer* GetRenderer() {
    return renderer;
}
