/*
Kokotän jutun ois tietenki voinu tehä yheksi classiksi.
Enpä tehny koska kyseessä on prototyyppi.
*/

#include <string>
#include <random>
#include "cell.h"

void DrawFPS(TTF_Font* font, Vec2i pos, SDL_Color color)
{
	std::string fps = "FPS: " + std::to_string(GetFPS());
	DrawText(fps.c_str(), font, pos, color, false);
}

// Updates
void UpdateSand(int x, int y)
{
	// Lambda eli ns mini functio
	auto sand_move = [&](Vec2i _other)
	{
		int fail_chance = 255;
		if (GetWeight({ x,y }) > (GetWeight(_other))) // Painavempi objecti liikkualemmas
		{
			fail_chance -= ((GetWeight({ x,y }) * 1.25f) - GetWeight(_other));
		}

		return MoveCell({ x,y }, _other, fail_chance);
	};

	if (sand_move({ x, y + 1 }))
		return;
	if (sand_move({ x + 1, y + 1 }))
		return;
	if (sand_move({ x - 1, y + 1 }))
		return;
}

void UpdateWood(int x, int y)
{
	// Wood is a stationary object, and is not meant to move.
}

void UpdateWater(int x, int y)
{
	// Veden liikkumiselle Lambda
	auto water_move = [&](Vec2i target_pos)
	{
		if (GetWeight({ x, y }) > GetWeight(target_pos)) //Toinen on kevyempi? jos on vaihatakeet paikkaa
		{
			if (GetCell(target_pos).type == CellType::Fire) // Paitsi jos se toine on tulta, poista tuli ja siirrä vesi siihe
				return ReplaceCell({ x,y }, target_pos);
			else
				return MoveCell({ x,y }, target_pos);
		}
		return false;
	};

	if (water_move({ x    , y + 1 }))
		return;
	if (water_move({ x + 1, y + 1 }))
		return;
	if (water_move({ x - 1, y + 1 }))
		return;

	/*
	Jos suoraan alaspäin tai alas viiston ei päässy, koita etsiä jokin paikka oikealta, sitten vasemmalta jonne mennä.
	Käyttää lifetime variablea hasusti, sunnan muistamiseen, en halunnu tehä sille omaa variablea, koska sitten se variable pitäs olla kaikilla
	Celleillä eikä vain vedellä.
	*/
	Cell& cur_cell = GetCell(x, y);
	if (abs(cur_cell.other) != 1) // Jos otheriin ei ole merkitty suuntaa
		cur_cell.other = 1;

	// Veden edestakasin meno
	if (GetCell(x, y + 1).type != CellType::Air && rand() % 2)
	{
		if (water_move({ x + cur_cell.other, y }))
			return;
		cur_cell.other *= -1;
	}
}

void UpdateSmoke(int x, int y)
{
	auto smoke_move = [&](Vec2i target_pos)
	{
		if (GetWeight({ x, y }) < GetWeight(target_pos))
			return MoveCell({ x,y }, target_pos);
		return false;
	};

	Cell& cell = GetCell(x, y);
	// Savun Haihtuminen
	if (rand() % 256 == 0)
	{
		cell.type = CellType::Air;
		return;
	}
	
	if (smoke_move({ x    , y - 1 }))
		return;
	if (smoke_move({ x + 1, y - 1 }))
		return;
	if (smoke_move({ x - 1, y - 1 }))
		return;
	if (GetWeight({ x, y }) < GetWeight({ x - 1, y }) && GetCell(x, y - 1).type != CellType::Air && rand() % 2 == 0)
	{
		if (MoveCell({ x,y }, { x - 1,y }))
			return;
	}
	if (GetWeight({ x, y }) < GetWeight({ x + 1, y }) && GetCell(x, y - 1).type != CellType::Air && rand() % 2 == 0)
	{
		if (MoveCell({ x,y }, { x + 1,y }))
			return;
	}
}

void UpdateFire(int x, int y)
{
	const Uint8 smoke_change = 3;

	// Create Smoke //
	if (smoke_change > rand() % 100 && GetCell(x, y-1).type == CellType::Air)
	{
		GetCell(x, y - 1).type = CellType::Smoke;
	}


randomize_again:
	/*
	Ottaa vierestä random cellin, katsoo
	kuink herkästi se voi palaa ja randomisoi että
	syttyykö se
	*/
	int i = rand() % 7 - 3; // -3 ja 3 väillä
	int j = rand() % 5 - 3; // -3 ja 1 väillä tuli menee tarkotuksellisesti herkemmin ylöspäin, kun alaspäin

	if (!InBoundaries({ x + i, y + j }))
		goto randomize_again;

	// Sytytä viereisen cellin? //
	if (GetFlamability({ x + i, y + j }) > rand() % 255)
	{
		GetCell({ x + i, y + j }).type = CellType::Fire;

		// Create Smoke
		j--;
		if (GetCell({ x + i, y + j }).type == CellType::Air)
			GetCell({ x + i, y + j }).type = CellType::Smoke;
	}
	else if (GetCell(x + i, x + j).type != CellType::Fire && rand() % 32 == 0)
	{
		GetCell({ x,y }).type = CellType::Air;
	}

	/*
	Käytä samaa liikkumis strategiaa kun hiekka,
	mutta koska tuli on kevyempää se liikkuu hitaammin alaspäin
	*/
	UpdateSand(x,y);
}

void UpdateGunPowder(int x, int y)
{
	auto gunpoweder_move = [&](Vec2i _other)
	{
		int fail_chance = 255;
		if (GetWeight({ x,y }) > (GetWeight(_other)))
		{
			fail_chance -= ((GetWeight({ x,y }) * 3) - GetWeight(_other));
		}

		return MoveCell({ x,y }, _other, fail_chance);
	};

	if (gunpoweder_move({ x + 0, y + 1 }))
		return;
	if (gunpoweder_move({ x + 1, y + 1 }))
		return;
	if (gunpoweder_move({ x - 1, y + 1 }))
		return;	
}

static TTF_Font*	s_cellfont;     // Fontti
static SDL_Surface* s_surface;      // SDL2sen cpu textuuri, joka piirretään näytölle
static SDL_Color*	s_screenpixels; // Näytölle piirretävän pixel data, jota s_surface käyttää
static Cell*		s_celltable;    // Pointer cell arrayhyn joka sisältää kaikki näytöllä näkyvät cellit
static CellType		s_current_cell = CellType::Sand; // Tällä hetkellä valitty celli, jonka voi piirtää näytölle

void UpdateCells()
{
	// Uuden "pöydän" luominen
	if (IsKeyDown(SDL_SCANCODE_RETURN))
	{
		for (int i = 0; i < FIELD_W * FIELD_H; i++)
		{
			s_celltable[i].type = (CellType)((rand() % 8 == 0) * (int)s_current_cell);
		}
	}
	if (IsKeyDown(SDL_SCANCODE_ESCAPE))
	{
		for (int i = 0; i < FIELD_W * FIELD_H; i++)
		{
			s_celltable[i].type = CellType::Air;
		}
	}

	// Cellin valinta
	if (IsKeyDown(SDL_SCANCODE_1))
		s_current_cell = CellType::Sand;
	if (IsKeyDown(SDL_SCANCODE_2))
		s_current_cell = CellType::Water;
	if (IsKeyDown(SDL_SCANCODE_3))
		s_current_cell = CellType::Wood;
	if (IsKeyDown(SDL_SCANCODE_4))
		s_current_cell = CellType::GunPowder;
	if (IsKeyDown(SDL_SCANCODE_5))
		s_current_cell = CellType::Fire;

	// Cellien laittaiminen ja poistaminen
	if (IsMouseKeyDown(SDL_BUTTON_LEFT))
	{
		Vec2i ms = GetMousePosition();
		if (InBoundaries(ms))
			GetCell(ms.x, ms.y).type = s_current_cell;
	}
	if (IsMouseKeyDown(SDL_BUTTON_RIGHT))
	{
		Vec2i ms = GetMousePosition();
		if (InBoundaries(ms))
			GetCell(ms.x, ms.y).type = CellType::Air;
	}

	// Ajan Edistäminen
	if (!IsKeyDown(SDL_SCANCODE_SPACE))
		return;

	for (int i = 0; i < FIELD_W * FIELD_H; i++)
	{
		s_celltable[i].is_updated = false;
	}

	for (int x = 0; x < FIELD_W; x++)
		for (int y = 0; y < FIELD_H; y++)
		{
			const Cell& cell = GetCell(x, y);
			if (cell.is_updated) // Don't update the same cell twice
				continue;

			switch (cell.type)
			{
			case CellType::Air:
				break;
			case CellType::Sand:
				UpdateSand(x, y);
				break;
			case CellType::Wood:
				UpdateWood(x, y);
				break;
			case CellType::Water:
				UpdateWater(x, y);
				break;
			case CellType::Smoke:
				UpdateSmoke(x, y);
				break;
			case CellType::Fire:
				UpdateFire(x, y);
				break;
			case CellType::GunPowder:
				UpdateGunPowder(x, y);
				break;
			default:
				break;
			}

		}
}

void DrawCells()
{
	// Pixel datan muokkamiseen lambda
	auto get_pixel = [](int x, int y) -> SDL_Color&
	{
		return s_screenpixels[y * FIELD_W + x];
	};
	
	// Muokkaa pixelin väri, elementistä riippuen
	for (int x = 0; x < FIELD_W; x++)
	{
		for (int y = 0; y < FIELD_H; y++)
		{
			switch (GetCell(x, y).type)
			{
			case CellType::Air:
				get_pixel(x,y) = WHITE;
				break;
			case CellType::Sand:
				get_pixel(x, y) = SAND_COLOR;
				break;
			case CellType::Wood:
				get_pixel(x, y) = WOOD_COLOR;
				break;
			case CellType::Water:
				get_pixel(x, y) = WATER_COLOR;
				break;
			case CellType::Smoke:
				get_pixel(x, y) = SMOKE_COLOR;
				break;
			case CellType::Fire:
				get_pixel(x, y) = FIRE_COLOR;
				break;
			case CellType::GunPowder:
				get_pixel(x, y) = GUNPOWDER_COLOR;
				break;
			}
		}
	}

	// Tee textuuri pixel datasta
	static SDL_Renderer* renderer = GetRenderer();
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, s_surface);

	// Piirrä texture
	DrawTexture(texture, {0,0});

	// Poista texture
	SDL_DestroyTexture(texture);
	
	PrintCellType(s_current_cell);
	DrawFPS(s_cellfont, {2, 2}, RED);
}

void PrintCellType(CellType type)
{
	const Vec2i pos = { FIELD_W/2, 80};
	switch (type)
	{
	case CellType::Air:
		DrawText("Ilma", s_cellfont, pos, RED, true);
		break;
	case CellType::Sand:
		DrawText("Hiekka", s_cellfont, pos, RED, true);
		break;
	case CellType::Water:
		DrawText("Vesi", s_cellfont, pos, RED, true);
		break;
	case CellType::Wood:
		DrawText("Puu", s_cellfont, pos, RED, true);
		break;
	case CellType::Smoke:
		DrawText("Savu", s_cellfont, pos, RED, true);
		break;
	case CellType::Fire:
		DrawText("Tuli", s_cellfont, pos, RED, true);
		break;
	case CellType::GunPowder:
		DrawText("Ruuti", s_cellfont, pos, RED, true);
		break;
	default:
		DrawText("Invaild", s_cellfont, pos, RED, true);
		break;
	}
}

void CellInit()
{
	s_cellfont = OpenFont("assets/font.ttf", 8);

	// Initalize Cell Array //
	s_celltable = new Cell[FIELD_W * FIELD_H];
	memset(s_celltable, 0, FIELD_W * FIELD_H * sizeof(Cell));

	// Create Pixel Buffer //
	const int depth = 32;
	const int pitch = 4 * FIELD_W;
	s_screenpixels = new SDL_Color[FIELD_W * FIELD_H];
	s_surface = SDL_CreateRGBSurfaceFrom(s_screenpixels,
		FIELD_W,    FIELD_H, depth, pitch,
		0x000000ff, 0x0000ff00, 0x00ff0000, 0);
}

void CellCleanup()
{
	TTF_CloseFont(s_cellfont);
	SDL_FreeSurface(s_surface);
	delete[] s_celltable;
	delete[] s_screenpixels;
}

// Utils //

bool ReplaceCell(Vec2i this_pos, Vec2i other_pos)
{
	if (!InBoundaries(this_pos))
		return false;
	if (!InBoundaries(other_pos))
		return false;
	if (GetWeight(other_pos) == 255)			   // not movable
		return false;

	Cell& _this  = GetCell(this_pos.x, this_pos.y);
	Cell& _other = GetCell(other_pos.x, other_pos.y);

	_this.is_updated = true;
	_other = _this;
	_this.type = CellType::Air;
	return true;
}

bool MoveCell(Vec2i this_pos, Vec2i other_pos, int chance)
{
	if (chance >= rand() % 256) // higher more likely to fail
		return false;
	if (!InBoundaries(this_pos))
		return false;
	if (!InBoundaries(other_pos))
		return false;
	if (GetWeight(other_pos) == 255)			   // not movable
		return false;
	if (GetCell(other_pos).type == CellType::Fire) // not movable
		return false;

	Cell& _this  = GetCell(this_pos.x, this_pos.y);
	Cell& _other = GetCell(other_pos.x, other_pos.y);
	Cell  hold = _other;

	_this.is_updated = true;
	_other = _this;
	_this = hold;
	return true;
}

bool InBoundaries(Vec2i pos)
{
	if (pos.x >= FIELD_W || pos.x < 0)
		return false;
	if (pos.y >= FIELD_H || pos.y < 0)
		return false;
	return true;
}

Cell& GetCell(Vec2i pos)
{
	return s_celltable[pos.y * FIELD_W + pos.x];
}

Cell& GetCell(int x, int y)
{
	return s_celltable[y * FIELD_W + x];
}

Uint8 GetWeight(Vec2i pos)
{
	CellType type = GetCell(pos).type;

	switch (type)
	{
	case CellType::Air:
		return 5;
	case CellType::Sand:
		return 70;
	case CellType::Wood:
		return 255;
	case CellType::Water:
		return 40;
	case CellType::Smoke:
		return 1;
	case CellType::Fire:
		return 6;
	case CellType::GunPowder:
		return 70;
	default:
		return 255;
	}
}

Uint8 GetFlamability(Vec2i pos)
{
	CellType type = GetCell(pos).type;

	switch (type)
	{
	case CellType::Air:
		return 0;
	case CellType::Sand:
		return 2;
	case CellType::Wood:
		return 50;
	case CellType::Water:
		return 0;
	case CellType::Smoke:
		return 0;
	case CellType::Fire:
		return 0;
	case CellType::GunPowder:
		return 255;
	default:
		return 0;
	}
}
