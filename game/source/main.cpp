#include "../engine/engine.h"
#include "cell.h"

int main(int argc, char** args)
{
	// Init Window & SDL2 //
	CreateWindow("Pixel Simulation", 1280, 720, SDL_WINDOW_SHOWN);
	SetRenderScale({8.0f, 8.0f});

	CellInit();
	SetTargetFPS(60);
	while (!ShouldWindowClose())
	{
		DoEvents(); // Hoitaa ikkunan eventit, kuten ikkunan sulkemisen, siirtämisen.
		
		// Update //
		UpdateCells();

		// Draw //
		ClearScreen(WHITE);
		DrawCells();
		RenderScreen();
	}

	CellCleanup();
	DestroyWindow();
	return 0;
}
