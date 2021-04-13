#include "../engine/engine.h"

#define FIELD_W 1280/8
#define FIELD_H 720/8

#define SAND_COLOR	SDL_Color{253, 165, 100, 255}
#define WATER_COLOR BLUE
#define WOOD_COLOR SDL_Color{84,52,35,255}
#define SMOKE_COLOR SDL_Color{140,140,140,255}
#define FIRE_COLOR SDL_Color{(Uint8)(200 + rand() % 55), (Uint8)(10 + rand() % 75), (Uint8)(10 + rand() % 25), 255} //Randomizes the color a bit
#define GUNPOWDER_COLOR SDL_Color{40,40,40,255}

enum class CellType
{
	Null = -1,
	Air = 0,
	Sand = 1,
	Wood = 2, // Stationary, but flamable
	Water = 3,
	Smoke = 4,
	Fire = 5,
	GunPowder = 6,
};

struct Cell
{
	CellType type;
	bool is_updated;
	int other = -1; //Used differently by different elements
};

// Updates //

void UpdateSand(int x, int y);
void UpdateWood(int x, int y);
void UpdateWater(int x, int y);
void UpdateSmoke(int x, int y);
void UpdateFire(int x, int y);
void UpdateGunPowder(int x, int y);

void UpdateCells();
void DrawCells();
void PrintCellType(CellType type);
void CellInit();
void CellCleanup();

// Utils

// If the given x,y coordinates are possible
bool InBoundaries(Vec2i pos);

/* switches positions with 2 cells, this cell is counted as "updated". Can also pass change to fail this (0 - 255)
Returns success */
bool MoveCell(Vec2i this_pos, Vec2i other_pos, int change_to_fail = 0);
/* other becomes this and this becomes air, this cell is counted as "updated"
Returns success */
bool ReplaceCell(Vec2i this_pos, Vec2i other_pos);

Cell& GetCell(Vec2i pos);
Cell& GetCell(int x, int y);

// Gets the weight of the given cell (0-255)
Uint8   GetWeight(Vec2i pos);
// Gets the flamability of the given cell (0-255) more is more flamable
Uint8   GetFlamability(Vec2i pos);