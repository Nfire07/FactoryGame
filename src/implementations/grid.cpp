#include "../headers/grid.h"
#include <raylib.h>
#include <iostream>
#include <time.h>

/*
 * Memory allocate an empty Grid given a size
 *  - id default 0
 *  - name default NULL
 */

Grid::Grid() {
	this->gridBlocks = nullptr;
	this->size = -1;
}

Grid::Grid(const int size) {
	srand(time(nullptr));

	this->size = size;
	this->gridBlocks = new GridBlock*[size];

	for(size_t i = 0;i < this->size;++i) {
		this->gridBlocks[i] = new GridBlock[size];
		for(size_t j = 0;j < this->size;++j) {
			this->gridBlocks[i][j] = {0, {biomeNames[rand() % 4], biomeColors[rand() % 4], .5f, nullptr, nullptr }};
			this->gridBlocks[i][j].id = 0;
			this->gridBlocks[i][j].biome = {biomeNames[rand() % 4], biomeColors[rand() % 4], .5f, nullptr, nullptr };
		}
	}
}

Grid::~Grid() {
	for(size_t i = 0;i < this->size;++i)
		free(this->gridBlocks[i]);
	free(this->gridBlocks);
}

void Grid::Print() const {
	printf("Biome(%d):\n",this->size);

	for(size_t i = 0;i < this->size;++i) {
		for(size_t j = 0;j < this->size;++j)
			std::cout << this->gridBlocks[i][j].id << ' ';
		std::cout << std::endl;
	}
}

void Grid::Draw(const int tileSize, const Color color) {
	constexpr int halfW = 2000;
	constexpr int halfH = 2000;

    for (int x = -halfW; x <= halfW; x += tileSize)
        DrawLine(x, -halfH, x, halfH, color);

    for (int y = -halfH; y <= halfH; y += tileSize)
        DrawLine(-halfW, y, halfW, y, color);
}