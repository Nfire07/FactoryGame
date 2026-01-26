#include <raylib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include "headers/biome.h"
#define RAYGUI_IMPLEMENTATION
#include "./headers/core/raygui.h"
#include "headers/grid.h"
#include "headers/guistyleimpl.h"


int screenWidth = 1920;
int screenHeight = 1080;
float zoom = 1.0f;
bool error = false;
int selectedBiome = BIOME_PLAINS;
Camera2D camera = { 0 };


/*
 * Graphics
 *  - a grid zoomable +-
 *  - a top bar with global resources and a way to navigate
 *  - a way to create insert destroy and manipulate blocks and place rails(e.g. Modals popups etc..)
 */

void Setup(const char* name) {
    InitWindow(screenWidth, screenHeight, name);
    int monitor = GetCurrentMonitor();
    screenWidth = GetMonitorWidth(monitor);
    screenHeight = GetMonitorHeight(monitor);
    SetWindowSize(screenWidth, screenHeight);
    SetWindowPosition(0, 0);
    SetWindowState(FLAG_FULLSCREEN_MODE);
    ShowCursor();
    SetTargetFPS(GetMonitorRefreshRate(monitor));
    camera.target = (Vector2){ 0, 0 };  
    camera.offset = (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

}

void Update(float delta) {}

void DrawGrid_temp(int tileSize, Color color) {
    const int halfW = 2000;
    const int halfH = 2000;

    for (int x = -halfW; x <= halfW; x += tileSize)
        DrawLine(x, -halfH, x, halfH, color);

    for (int y = -halfH; y <= halfH; y += tileSize)
        DrawLine(-halfW, y, halfW, y, color);
}

void DrawMap(const Grid grid) {
    for(size_t i = 0; i < grid.size; ++i) {
        for(size_t j = 0; j < grid.size; ++j) {
            DrawRectangle(i * 50, j * 50, 50, 50, grid.gridBlocks[i][j].biome.color);
        }
    }
}

void GuiTopBar(float heightPerc,float paddingPerc,float buttonWidthPerc,float buttonHeightPerc,float buttonYPerc,float spacingPerc,float startXPerc) {
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
        "Plains;Mountains;Hills;Sea",
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
        0.01f,
        0.10f,
        0.4f,
        2.0f,
        0.02f,
        2.0f
    );
}

void Render() {
    BeginDrawing();
    
	ClearBackground(GetColor(0x212121ff));
    
    BeginMode2D(camera);
	
    DrawGrid_temp(50, GRAY);

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
}

int main(void) {
    Setup("Factory");
    
    error = LoadCustomGuiStyle();

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
            Resize(0, 0);
    }   
    
    Close();
    return 0;
}