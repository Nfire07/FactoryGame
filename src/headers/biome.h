#pragma once

/*
 * This struct contains the template for a structure placed in the biome's grid
 * - The id is used to draw it on the screen during render
 * - id == 0 means that the tile is free
 * - name is used to give more info and if the tile is free would be NULL
 */
typedef struct GridBlock_s{
	int id;
	char* name;
}GridBlock;

/*
 * This struct contains the grid elements
 * - size contains the len of the grid in tiles
 */
typedef struct Grid_s{
	GridBlock** gridBlocks;
	int size;
}Grid;

typedef struct Biome_s{
	Grid grid;
	int occupied;
}Biome;

Biome AllocateBiome(int size);
void PrintBiome_debug(Biome biome);
void FreeBiome(Biome *biome);

