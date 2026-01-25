#include <raylib.h>
#include <stdio.h>
#include <stdbool.h>

#define RAYGUI_IMPLEMENTATION
#include "./headers/core/raygui.h"
#include "headers/grid.h"

int screenWidth = 0;
int screenHeight = 0;
bool error = false;

/*
 * Graphics
 *  - a grid zoomable +-
 *  - a top bar with global resources and a way to navigate
 *  - a way to create insert destroy and manipulate blocks and place rails(e.g. Modals popups etc..)
 *
 * 4 biomes
 * 
 * - plains
 * - mountains
 * - sea
 * - hills
 */

int Setup(const char* name) {
	InitWindow(screenWidth,screenHeight,name);

	screenWidth = GetScreenWidth();
	screenHeight = GetScreenHeight();

	ShowCursor();
	ToggleFullscreen();

	SetTargetFPS(60);

}

void Update(float delta) {

}

void DrawGrid_temp(const int tileSize, int dx, int dy, const Color gridColor) {

	for(int i=1;i<screenHeight/tileSize + 1;i++)
		DrawLine(0,i*tileSize,screenWidth,i*tileSize,gridColor);

	for(int i=1;i<screenWidth/tileSize + 1;i++)
		DrawLine(i*tileSize,0,i*tileSize,screenHeight,gridColor);
}

void DrawMap(const Grid grid) {
	for(size_t i = 0;i < grid.size;++i) {
		for(size_t j = 0;j < grid.size;++j) {
			DrawRectangle(i * 50, j * 50, 50, 50, grid.gridBlocks[i][j].biome.color);
		}
	}
}

void Render(const Grid grid) {
	BeginDrawing();

	ClearBackground(GetColor(0x212121ff));
	DrawGrid_temp(50,0,0,GRAY);
	DrawMap(grid);

	EndDrawing();	
}

void Resize(int width,int height) {

}

void Input() {

}

void Close() {
	
	CloseWindow();
}

int main(void) {
	// GuiLoadStyle("./assets/guistyle.rgs");
	Setup("Factory");

	if(error) {
		printf("ERROR:	invalid setup!\n");
		return 1;
	}

	const Grid grid = CreateGrid(15);
	PrintGrid_debug(grid);

	while(!WindowShouldClose()) {
		const float delta = GetFrameTime();

		Update(delta);

		Render(grid);

		Input();

		if(IsWindowResized())
			Resize(0,0);
	}	

	Close();

	return 0;
}