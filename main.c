#include <stdio.h>
#include <raylib.h>

int main(void){
	
	InitWindow(500,500,"Factory Game");

	while(!WindowShouldClose()){
		if(IsKeyPressed(22)) break;
		BeginDrawing();
		ClearBackground(RAYWHITE);
		EndDrawing();
	}	

	CloseWindow();

	return 0;
}

