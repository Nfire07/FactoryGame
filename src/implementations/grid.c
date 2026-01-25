#include "../headers/grid.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * Memory allocate an empty Grid given a size
 *  - id default 0
 *  - name default NULL
 */
Grid CreateGrid(const int size) {
	srand(time(NULL));
	Grid grid;

	grid.size = size;
	grid.gridBlocks = MemAlloc(size*sizeof(GridBlock));

	for(int i = 0; i < grid.size ; i++) {
		grid.gridBlocks[i] = MemAlloc(size * sizeof(GridBlock));

		for(int j = 0; j < grid.size; j++) {
			grid.gridBlocks[i][j].id = 0;
		    grid.gridBlocks[i][j].biome = (Biome) {biomeNames[rand() % 4], biomeColors[rand() % 4], .5f, NULL, NULL };
		}
	}

	return grid;
}

void FreeGrid(const Grid* grid) {
	for(int i=0;i<grid->size;i++)
		MemFree(grid->gridBlocks[i]);

	MemFree(grid->gridBlocks);
}

void PrintGrid_debug(Grid grid) {
	printf("Biome(%d):\n",grid.size);

	for(int i=0;i<grid.size;i++) {
		for(int j=0;j<grid.size;j++) {
			printf("%d ",grid.gridBlocks[i][j].id);
		}
		printf("\n");
	}
}