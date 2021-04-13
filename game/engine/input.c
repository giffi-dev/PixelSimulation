#include "input.h"
#include "renderer.h"

const Uint8* KeyboardStates;

bool IsKeyDown(SDL_Scancode key) {
	return KeyboardStates[key];
}

Vec2i GetMousePosition(void) {
	int mx, my;
	SDL_GetMouseState(&mx, &my);
	
	// Takes in count Window's render scale //
	Vec2f rs = GetRenderScale();
	mx /= (int)rs.x;
	my /= (int)rs.y;

	return (Vec2i) { mx, my };
}

bool IsMouseKeyDown(int key) {
	return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(key);
}

bool IsMouseOver(SDL_Rect sh) {
	Vec2i mp = GetMousePosition();

	return	(mp.x >= sh.x && mp.x < sh.x + sh.w) &&
			(mp.y >= sh.y && mp.y < sh.y + sh.h);
}


// CONTROLLERS //


SDL_GameController* controller = NULL;

void InitController() {
	controller = SDL_GameControllerOpen(0);
}

void CloseController() {
	if (controller != NULL)
	{
		SDL_GameControllerClose(controller);
	}
}

bool IsControllerKeyDown(int key)
{
	return SDL_GameControllerGetButton(controller, (SDL_GameControllerButton)key) == 1;
}

/*
int rand_range(int min, int max){
   return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}
*/