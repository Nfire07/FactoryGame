#pragma once
#include "biome.h"
#include <raylib.h>
/*
 * This struct contains the template for a structure placed in the biome's grid
 * - The id is used to draw it on the screen during render
 * - id == 0 means that the tile is free
 * - name is used to give more info and if the tile is free would be NULL
 */

class GridBlock {
public:
	int id;
	Biome biome;

	GridBlock() {
		this->id = -1;
		this->biome = {};
	}

	GridBlock(const int id, const Biome &biome) {
		this->id = id;
		this->biome = biome;
	}
};

/*
 * This struct contains the grid elements
 * - size contains the len of the grid in tiles
 */

class Grid {
public:
	GridBlock** gridBlocks;
	int size;

	Grid();
	explicit Grid(int size);
	~Grid();
	void Print() const;
	static void Draw(int tileSize, Color color);
};