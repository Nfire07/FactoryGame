#include <raylib.h>
#include <stdio.h>

int screenWidth = 0;
int screenHeight = 0;

/*
 * Graphics
 *  - a grid zoomable +-
 *  - a top bar with global resources and a way to navigate
 *  - a way to create insert destroy and manipulate blocks and place rails
 *
 * We need 4 biomes
 * 
 * plains
 * mountains
 * sea
 * hills
 *
 */

void Setup(const char* name){
	int monitor = GetCurrentMonitor();

	screenWidth = 2560;
        screenHeight = 1440;

	InitWindow(screenWidth,screenHeight,name);

	ToggleFullscreen();

	SetTargetFPS(60);
}

void Update(float delta){

}

void DrawGrid(int tileSize, int dx, int dy, Color gridColor){
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
	DrawGrid(50,0,0,GRAY);

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

