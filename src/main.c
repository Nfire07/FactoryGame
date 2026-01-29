#define RAYGUI_IMPLEMENTATION
#include "raylib.h"
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <direct.h>
#include "headers/biome.h"
#include "headers/core/raygui.h"
#include "headers/generation.h"
#include "headers/guistyleimpl.h"


int screenWidth = 1920;
int screenHeight = 1080;

float zoom = 1.0f;
bool error = false;
int selectedBiome = BIOME_PLAINS;
size_t mapsize = 160;
int** generatedMap;
Texture2D plainsTexture;
Camera2D camera = { 0 };

/*
 * Graphics
 *  - a grid zoomable +-
 *  - a top bar with global resources and a way to navigate
 *  - a way to create insert destroy and manipulate blocks and place rails(e.g. Modals popups etc..)
 */

void Setup(const char* name) {
    SetConfigFlags(FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_MAXIMIZED | FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, name);
    const int monitor = GetCurrentMonitor();
    SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
    ShowCursor();

    camera.target = (Vector2){ 0, 0 };  
    camera.offset = (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

}

void Update(float delta) {}

void GuiTopBar(float heightPerc,float paddingPerc,float buttonWidthPerc,float buttonHeightPerc,float buttonYPerc,float spacingPerc,float startXPerc) {
    screenWidth = GetRenderWidth();
    screenHeight = GetRenderHeight();

    const float topBarHeight = screenHeight * heightPerc;  
    const float padding = screenWidth * paddingPerc;        
    
    DrawRectangle(0, 0, screenWidth, topBarHeight, GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
    DrawLineEx((Vector2){0, topBarHeight}, (Vector2){screenWidth, topBarHeight}, 2, GetColor(GuiGetStyle(DEFAULT,BORDER_COLOR_NORMAL)));
    
    const float buttonWidth = screenWidth * buttonWidthPerc;    
    const float buttonHeight = topBarHeight * buttonHeightPerc;   
    const float buttonY = (topBarHeight - buttonHeight) / buttonYPerc;  
    const float buttonSpacing = screenWidth * spacingPerc;  
    
    const float totalButtonsWidth = (buttonWidth * 4) + (buttonSpacing * 3);
    float startX = (screenWidth - totalButtonsWidth) / startXPerc;
    
    int previousBiome = selectedBiome;
    
    GuiToggleGroup(
        (Rectangle){startX, buttonY, buttonWidth, buttonHeight},
        "Factory;Mountains;Hills;Sea",
        &selectedBiome
    );
    
    
    if (previousBiome != selectedBiome) {
        const char* biomes[] = {"Plains", "Mountains", "Hills", "Sea"};
        printf("%s biome selected\n", biomes[selectedBiome]);
    }
}

void Gui(){
	GuiTopBar(
        0.08f,
        1.1f,
        0.12f,
        0.4f,
        2.0f,
        0.02f,
        2.0f
    );
}

void DrawGenerated_temp(){
    for(int i=0;i<mapsize;i++){
        for(int j=0;j<mapsize;j++){
            const int x = i - 80;
            const int y = j - 80;
            if(!generatedMap[i][j])
                DrawTextureEx(plainsTexture,(Vector2){x*25,y*25},0,25.f/plainsTexture.width,WHITE);
            else
                DrawRectangle(x*25, y*25,25,25,biomeColors[generatedMap[i][j]]);
        }
    }
}


void Render() {
    BeginDrawing();
    
	ClearBackground(GetColor(0x212121ff));
    
    BeginMode2D(camera);
	
    DrawGenerated_temp();
    
    EndMode2D();
    
	Gui();

    EndDrawing();
}

void Resize(int width, int height) {}

void Input() {
    float wheel = GetMouseWheelMove();
    if (wheel != 0) {
        camera.zoom += wheel * 0.1f;
        if (camera.zoom < 0.2f) camera.zoom = 0.2f;
        if (camera.zoom > 3.0f) camera.zoom = 3.0f;
    }

    if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
        Vector2 delta = GetMouseDelta();
        camera.target.x -= delta.x / camera.zoom;
        camera.target.y -= delta.y / camera.zoom;
    }
}

void Close() {
    CloseWindow();
    for (size_t i = 0; i < 10; i++)
        MemFree(generatedMap[i]);

    MemFree(generatedMap);
}

int main(void) {

    char cwd[1024];
    if (_getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working dir: %s\n", cwd);
    } else {
        perror("_getcwd() error");
    }

    srand(time(NULL));
    Setup("Factory");
    
    error = LoadCustomGuiStyle();
    generatedMap = generateRandomMapFromSeed(mapsize,time(NULL));
    plainsTexture = LoadTexture("../assets/Plains1.png");


    if(error) {
        printf("ERROR: invalid setup!\n");
        return 1;
    }
    
    while(!WindowShouldClose()) {
        const float delta = GetFrameTime();

        Update(delta);

        Render();

        Input();

        if(IsWindowResized())
            Resize(GetRenderWidth(),GetRenderHeight());
    }
    
    UnloadTexture(plainsTexture);
    Close();
    return 0;
}