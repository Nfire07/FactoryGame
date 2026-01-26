#include "headers/biome.h"
#include <raylib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#define RAYGUI_IMPLEMENTATION
#include "./headers/core/raygui.h"
#include "./headers/core/guistyle.h"
#include "headers/grid.h"


int screenWidth = 1920;
int screenHeight = 1080;
bool error = false;
int selectedBiome = BIOME_PLAINS;

/*
 * Graphics
 *  - a grid zoomable +-
 *  - a top bar with global resources and a way to navigate
 *  - a way to create insert destroy and manipulate blocks and place rails(e.g. Modals popups etc..)
 */

typedef struct {
    bool completed;
    bool success;
} StyleLoadStatus;

void* LoadStyleThreadFunction(void* arg) {
    StyleLoadStatus* status = (StyleLoadStatus*)arg;
    
    printf("INFO: Starting GUI style loading...\n");
    
    GuiLoadStyleGuistyle();
    
    printf("INFO: GUI style loaded successfully!\n");
    
    status->completed = true;
    status->success = true;
    
    return NULL;
}

void Setup(const char* name) {
    InitWindow(screenWidth, screenHeight, name);
    int monitor = GetCurrentMonitor();
    screenWidth = GetMonitorWidth(monitor);
    screenHeight = GetMonitorHeight(monitor);
    SetWindowSize(screenWidth, screenHeight);
    SetWindowPosition(0, 0);
    SetWindowState(FLAG_FULLSCREEN_MODE);
    ShowCursor();
    SetTargetFPS(60);
}

void Update(float delta) {
}

void DrawGrid_temp(const int tileSize, int dx, int dy, const Color gridColor) {
    for(int i=1; i<screenHeight/tileSize + 1; i++)
        DrawLine(0, i*tileSize, screenWidth, i*tileSize, gridColor);
    for(int i=1; i<screenWidth/tileSize + 1; i++)
        DrawLine(i*tileSize, 0, i*tileSize, screenHeight, gridColor);
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
    
	DrawGrid_temp(50, 0, 0, GRAY);
    
	Gui();

    EndDrawing();   
}

void Resize(int width, int height) {

}

void Input() {

}

void Close() {
    CloseWindow();
}

int main(void) {
    Setup("Factory");
    
    if(error) {
        printf("ERROR: invalid setup!\n");
        return 1;
    }
    
    StyleLoadStatus styleStatus = {false, false};
    
    pthread_t styleThread;
    printf("INFO: Creating thread for style loading...\n");
    
    if (pthread_create(&styleThread, NULL, LoadStyleThreadFunction, &styleStatus) != 0) {
        fprintf(stderr, "ERROR: Failed to create style loading thread!\n");
        error = true;
        return 1;
    }
    
    printf("INFO: Waiting for style loading to complete...\n");
    pthread_join(styleThread, NULL);
    
    if (!styleStatus.completed || !styleStatus.success) {
        fprintf(stderr, "ERROR: Style loading failed!\n");
        error = true;
        return 1;
    }
    
    printf("INFO: Style loaded successfully, continuing initialization...\n");
    
    
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
