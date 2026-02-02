#define RAYGUI_IMPLEMENTATION
#include "raylib.h"
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
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
Camera2D camera = {};

StyleLoad styleLoad = {};
Generation generation = {};

float elevationSea = 0.3f;
float elevationPlains = 0.55f;
float moistureHills = 0.45f;

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

void Update(const float delta) {}

void GuiTopBar(const float heightPerc, const float paddingPerc, const float buttonWidthPerc, const float buttonHeightPerc, const float buttonYPerc, const float spacingPerc, const float startXPerc) {
    screenWidth = GetRenderWidth();
    screenHeight = GetRenderHeight();

    const float topBarHeight = screenHeight * heightPerc;
    // const float padding = screenWidth * paddingPerc;
    
    DrawRectangle(0, 0, screenWidth, topBarHeight, GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
    DrawLineEx({0, topBarHeight}, {static_cast<float>(screenWidth), topBarHeight}, 2, GetColor(GuiGetStyle(DEFAULT, BORDER_COLOR_NORMAL)));
    
    const float buttonWidth = screenWidth * buttonWidthPerc;    
    const float buttonHeight = topBarHeight * buttonHeightPerc;   
    const float buttonY = (topBarHeight - buttonHeight) / buttonYPerc;  
    const float buttonSpacing = screenWidth * spacingPerc;  
    
    const float totalButtonsWidth = (buttonWidth * 4) + (buttonSpacing * 3);
    const float startX = (screenWidth - totalButtonsWidth) / startXPerc;

    const int previousBiome = selectedBiome;
    
    GuiToggleGroup((Rectangle){ startX, buttonY, buttonWidth, buttonHeight }, "Factory;Mountains;Hills;Sea", &selectedBiome);
    
    if (previousBiome != selectedBiome) {
        const char* biomes[] = {"Plains", "Mountains", "Hills", "Sea"};
        printf("%s biome selected\n", biomes[selectedBiome]);
    }
}

void LateralRandomSliders() {
    DrawRectangle(screenWidth - 150, 0, 150, screenHeight, GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
    DrawLineEx({static_cast<float>(screenWidth - 150), 0}, {static_cast<float>(screenWidth - 150), static_cast<float>(screenHeight)}, 2, GetColor(GuiGetStyle(DEFAULT,BORDER_COLOR_NORMAL)));

    GuiDrawText("Elevation Sea", {static_cast<float>(screenWidth - 150), 75, 150, 30}, TEXT_ALIGN_CENTER, WHITE);
    const bool changeSea = GuiSliderBar({static_cast<float>(screenWidth - 125), 100, 100, 30 }, "", "", &elevationSea, 0, 1);

    GuiDrawText("Elevation Plains", {static_cast<float>(screenWidth - 150), 125, 150, 30}, TEXT_ALIGN_CENTER, WHITE);
    const bool changePlains = GuiSliderBar({static_cast<float>(screenWidth - 125), 150, 100, 30 }, "", "", &elevationPlains, 0, 1);

    GuiDrawText("Moisture Hills", {static_cast<float>(screenWidth - 150), 175, 150, 30}, TEXT_ALIGN_CENTER, WHITE);
    const bool changeHills = GuiSliderBar({static_cast<float>(screenWidth - 125), 200, 100, 30 }, "", "", &moistureHills, 0, 1);

    if (changeHills || changePlains || changeSea)
        generatedMap = generation.generateRandomMapFromSeed(mapsize,time(nullptr), elevationSea, elevationPlains, moistureHills);
}

void Gui(){
	GuiTopBar(0.08f, 1.1f, 0.12f, 0.4f, 2.0f, 0.02f, 2.0f);
    LateralRandomSliders();
}

void DrawGenerated_temp(){
    for(size_t i = 0;i < mapsize;++i){
        for(size_t j = 0;j < mapsize;++j){
            const int x = i - 80;
            const int y = j - 80;
            if(!generatedMap[i][j])
                DrawTextureEx(plainsTexture, { static_cast<float>(x * 25), static_cast<float>(y * 25) }, 0, 25.f / plainsTexture.width, WHITE);
            else
                DrawRectangle(static_cast<float>(x * 25), static_cast<float>(y * 25), 25, 25, biomeColors[generatedMap[i][j]]);
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

void Resize(const int width, const int height) {}

void Input() {
    if (const float wheel = GetMouseWheelMove(); wheel != 0) {
        camera.zoom += wheel * 0.1f;
        if (camera.zoom < 0.2f) camera.zoom = 0.2f;
        if (camera.zoom > 3.0f) camera.zoom = 3.0f;
    }

    if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
        const Vector2 delta = GetMouseDelta();
        camera.target.x -= delta.x / camera.zoom;
        camera.target.y -= delta.y / camera.zoom;
    }
}

void Close() {
    CloseWindow();
    for (size_t i = 0;i < 10;i++)
        MemFree(generatedMap[i]);

    MemFree(generatedMap);
}

int main() {
    srand(time(nullptr));
    Setup("Factory");
    
    error = styleLoad.LoadCustomGuiStyle();
    generatedMap = generation.generateRandomMapFromSeed(mapsize,time(nullptr), elevationSea, elevationPlains, moistureHills);
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

    printf("Sea: %f\nPlains: %f\nHills: %f", elevationSea, elevationPlains, moistureHills);
    return 0;
}