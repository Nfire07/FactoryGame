#include <raylib.h>
#include <stdio.h>
#include <stdbool.h>
#include "headers/biome.h"

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

int Setup(const char* name){
	InitWindow(screenWidth,screenHeight,name);

	screenWidth = GetScreenWidth();
	screenHeight = GetScreenHeight();

	ShowCursor();
	ToggleFullscreen();

	SetTargetFPS(60);

}

void Update(float delta){

}

void DrawGrid_temp(int tileSize, int dx, int dy, Color gridColor){

	for(int i=1;i<screenHeight/tileSize + 1;i++){

		DrawLine(0,i*tileSize,screenWidth,i*tileSize,gridColor);

	}

	for(int i=1;i<screenWidth/tileSize + 1;i++){

		DrawLine(i*tileSize,0,i*tileSize,screenHeight,gridColor);

	}
}

void Render(){
	BeginDrawing();

	ClearBackground(GetColor(0x212121ff));
	DrawGrid_temp(50,0,0,GRAY);

	EndDrawing();	
}

void Resize(int width,int height){

}

void Input(){

}

void Close(){
	
	CloseWindow();
}

int main(void){
	Setup("Factory");

	if(error){
		printf("ERROR:	invalid setup!\n");
		return 1;
	}

	while(!WindowShouldClose()){
		float delta = GetFrameTime();

		Update(delta);

		Render();

		Input();

		if(IsWindowResized())
			Resize(0,0);
	}	

	Close();

	return 0;
}

