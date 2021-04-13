#include "window.h"

extern const Uint8* KeyboardStates;
SDL_Renderer* renderer = NULL;
SDL_Window* window = NULL;
bool close_window = false;
int s_targetfps = 1;

void CreateWindow(const char* title, int width, int height, SDL_WindowFlags flags) {

	// SWTICH //
	#ifdef __SWITCH__
		romfsInit();
		chdir("romfs:/");
	#endif

	// Init SDL //
	GIF_ASSERT(!SDL_Init(SDL_INIT_EVERYTHING),  SDL_GetError());
	GIF_ASSERT( IMG_Init(IMG_INIT_PNG),			SDL_GetError());
#ifndef NO_TTF
	GIF_ASSERT(!TTF_Init(),						SDL_GetError());
#endif
	// Window //
	window   = SDL_CreateWindow  (title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
	GIF_ASSERT(window, SDL_GetError());
	
	// Renderer //
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	GIF_ASSERT(renderer, SDL_GetError());

	KeyboardStates = SDL_GetKeyboardState(NULL);

	printf("Window Created\n");
}

void DestroyWindow() {
	SDL_DestroyWindow  (window);
	SDL_DestroyRenderer(renderer);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	printf("Window Destroyed\n");
}

int start_ticks = 0;
void DoEvents() {
	start_ticks = SDL_GetTicks();
	
	SDL_Event ev;
	while (SDL_PollEvent(&ev)) {
		switch (ev.type) {
		case SDL_QUIT:
			close_window = true; break;
		default:
			break;
		}
	}
}

extern void CalcFrameTimes(); // in fps_calc.c
void RenderScreen() {
	SDL_RenderPresent(renderer);

	CalcFrameTimes();
	
	if (s_targetfps == 0)
	{
		return;
	}
	int end_ticks = SDL_GetTicks() - start_ticks;
	int delay = (1000/s_targetfps) - end_ticks;

	if (delay > 0)
	{
		SDL_Delay(delay);
	}
}

void ClearScreen(SDL_Color color) {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(renderer);
}

bool ShouldWindowClose() {
	return close_window;
}

// Setters & Getters //

void ToggleFullscreen() {
	SDL_SetWindowFullscreen(window, !(SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN));
}

void SetFullscreen(bool state) {
	SDL_SetWindowFullscreen(window, state);
}

bool GetFullscreen() {
	return (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN);
}


void SetWindowSize(Vec2i size) {

}

Vec2i GetWindowSize()
{
	Vec2i size;
	SDL_GetWindowSize(window, &size.x, &size.y);
	return size;
}

void SetTargetFPS(int fps) {
	s_targetfps = fps;
}

int  GetTargetFPS() {
	return s_targetfps;
}
