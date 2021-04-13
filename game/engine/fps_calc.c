// FPS SHIT //
#include "fps_calc.h"
#define FPS_TIME_MARGIN 250
uint32_t frame_times[FPS_TIME_MARGIN];


// Makes an array of resent frametimes with a Array size of FPS_TIME_MARGIN
void CalcFrameTimes()
{
	static int idx = 0;
	static uint32_t prev;

	// FPS CALC //
	frame_times[idx] = SDL_GetTicks() - prev;
	prev = SDL_GetTicks();
	idx += 1;

	if (idx >= FPS_TIME_MARGIN)
	{
		idx = 0;
	}
	//          //
}

int GetFPS()
{
	int frames = 0;
	for (int i = 0; i < FPS_TIME_MARGIN; i++)
	{
		frames += frame_times[i];
	}

	return 1.0f / ((frames / (FPS_TIME_MARGIN * 1.0f)) / 1000.0f);
}

float GetDelta() {
	return 0.0;
}