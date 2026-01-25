#include "../headers/biome.h"
#include <raylib.h>
#include <stdio.h>

/*
 * Memory allocate an empty Biome given a size
 *  - id default 0
 *  - name default NULL
 */
Biome AllocateBiome(int size){
	Grid grid;

	grid.size = size;
	grid.gridBlocks = MemAlloc(size*sizeof(GridBlock));

	for(int i = 0; i < grid.size ; i++){
		grid.gridBlocks[i] = MemAlloc(size * sizeof(GridBlock));

		for(int j = 0; j < grid.size; j++){

			grid.gridBlocks[i][j].id = 0;
		        grid.gridBlocks[i][j].name = NULL;	

		}

	}

	Biome biome;
	biome.grid = grid;
	biome.occupied = 0;

	return biome;
}

void FreeBiome(Biome* biome){
	Grid grid = biome->grid;

	for(int i=0;i<grid.size;i++){

		MemFree(grid.gridBlocks[i]);

	}

	MemFree(grid.gridBlocks);
}

void PrintBiome_debug(Biome biome){
	printf("Biome(%d):\n",biome.grid.size);

	for(int i=0;i<biome.grid.size;i++){
		for(int j=0;j<biome.grid.size;j++){
			printf("%d ",biome.grid.gridBlocks[i][j].id);
		}
		printf("\n");
	}
}
