#include "buttons.h"
#include "../input.h"


extern SDL_Renderer* renderer;

bool CreateButton(SDL_Rect shape, bool make_visible) {
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		
	if (make_visible)
	{
		SDL_RenderDrawRect(renderer, &shape);
	}
		
	if (IsMouseOver(shape) && IsMouseKeyDown(SDL_BUTTON_LEFT))
	{
		if (make_visible)
			SDL_RenderFillRect(renderer, &shape);
		return true;
	}
	return false;
}