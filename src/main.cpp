#define RAYGUI_IMPLEMENTATION
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "headers/biome.h"
#include "headers/core/raygui.h"
#include "headers/generation.h"
#include "headers/guistyleimpl.h"

int screenWidth  = 1920;
int screenHeight = 1080;

bool error         = false;
int  selectedBiome = BIOME_PLAINS;

const size_t MAPSIZE    = 160;
const int    TILE_SIZE  = 25;
const int    MAP_OFFSET = static_cast<int>(MAPSIZE) / 2;

int**      generatedMap = nullptr;
Camera2D   camera      = {};
int        currentSeed = 1776669183;

StyleLoad  styleLoad  = {};
Generation generation = {};

float elevationSea    = 0.21f;
float elevationPlains = 0.43f;
float moistureHills   = 0.0f;

// Bioma attualmente sotto il cursore (-1 = nessuno)
int hoveredBiome = -1;

// ─── Prototipi ────────────────────────────────────────────────────────────────
void Setup(const char* name);
void Update(float delta);
void Render();
void Input();
void Close();
void Gui();
void GuiTopBar(float heightPerc, float paddingPerc, float buttonWidthPerc,
               float buttonHeightPerc, float buttonYPerc,
               float spacingPerc, float startXPerc);
void LateralRandomSliders();
void DrawGeneratedMap();
void RegenerateMap(bool newSeed);
void UpdateHoveredBiome();

// ─── Setup ────────────────────────────────────────────────────────────────────
void Setup(const char* name) {
    SetConfigFlags(FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_MAXIMIZED | FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, name);
    const int monitor = GetCurrentMonitor();
    SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
    ShowCursor();
    camera.target   = { 0.0f, 0.0f };
    camera.offset   = { screenWidth / 2.0f, screenHeight / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom     = 1.0f;
}

// ─── Generazione ─────────────────────────────────────────────────────────────
void RegenerateMap(const bool newSeed) {
    if (generatedMap != nullptr) {
        for (size_t i = 0; i < MAPSIZE; ++i)
            delete[] generatedMap[i];
        delete[] generatedMap;
        generatedMap = nullptr;
    }
    //if (newSeed)
    //    currentSeed = static_cast<int>(time(nullptr));
    generatedMap = generation.generateRandomMapFromSeed(
        MAPSIZE, currentSeed, elevationSea, elevationPlains, moistureHills);
}

// ─── Hover bioma ─────────────────────────────────────────────────────────────
void UpdateHoveredBiome() {
    if (generatedMap == nullptr) { hoveredBiome = -1; return; }

    // Converti posizione mouse in world space
    const Vector2 mouseScreen = GetMousePosition();
    const Vector2 mouseWorld  = GetScreenToWorld2D(mouseScreen, camera);

    // Ricava indice tile
    const int tileX = static_cast<int>(floorf(mouseWorld.x / TILE_SIZE)) + MAP_OFFSET;
    const int tileY = static_cast<int>(floorf(mouseWorld.y / TILE_SIZE)) + MAP_OFFSET;

    if (tileX >= 0 && tileX < static_cast<int>(MAPSIZE) &&
        tileY >= 0 && tileY < static_cast<int>(MAPSIZE))
        hoveredBiome = generatedMap[tileX][tileY];
    else
        hoveredBiome = -1;
}

// ─── Update ──────────────────────────────────────────────────────────────────
void Update(const float delta) {
    (void)delta;
    UpdateHoveredBiome();
}

// ─── Disegno mappa ───────────────────────────────────────────────────────────
void DrawGeneratedMap() {
    if (generatedMap == nullptr) return;
    for (size_t i = 0; i < MAPSIZE; ++i) {
        for (size_t j = 0; j < MAPSIZE; ++j) {
            const int x     = (static_cast<int>(i) - MAP_OFFSET) * TILE_SIZE;
            const int y     = (static_cast<int>(j) - MAP_OFFSET) * TILE_SIZE;
            const int biome = generatedMap[i][j];
            DrawRectangle(x, y, TILE_SIZE, TILE_SIZE, biomeColors[biome]);
        }
    }
}

// ─── GUI: top bar ────────────────────────────────────────────────────────────
// void GuiTopBar(const float heightPerc, const float paddingPerc,
//                const float buttonWidthPerc, const float buttonHeightPerc,
//                const float buttonYPerc, const float spacingPerc,
//                const float startXPerc) {
//     screenWidth  = GetRenderWidth();
//     screenHeight = GetRenderHeight();
//     const float topBarHeight  = screenHeight * heightPerc;
//     DrawRectangle(0, 0, screenWidth, static_cast<int>(topBarHeight),
//                   GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
//     DrawLineEx({ 0, topBarHeight }, { static_cast<float>(screenWidth), topBarHeight },
//                2, GetColor(GuiGetStyle(DEFAULT, BORDER_COLOR_NORMAL)));
//     const float buttonWidth   = screenWidth  * buttonWidthPerc;
//     const float buttonHeight  = topBarHeight * buttonHeightPerc;
//     const float buttonY       = (topBarHeight - buttonHeight) / buttonYPerc;
//     const float buttonSpacing = screenWidth  * spacingPerc;
//     (void)paddingPerc;
//     const float totalButtonsWidth = buttonWidth * 4 + buttonSpacing * 3;
//     const float startX = (screenWidth - totalButtonsWidth) / startXPerc;
//     const int previousBiome = selectedBiome;
//     GuiToggleGroup({ startX, buttonY, buttonWidth, buttonHeight },
//                    "Plains;Mountains;Hills;Sea", &selectedBiome);
//     if (previousBiome != selectedBiome)
//         printf("%s biome selected\n", biomeNames[selectedBiome]);
// }

// ─── GUI: slider laterali ────────────────────────────────────────────────────
void LateralRandomSliders() {
    const int panelWidth = 200;
    const int panelX     = screenWidth - panelWidth;
    const int padding    = 12;
    const int sliderW    = panelWidth - padding * 2;
    bool changed         = false;

    DrawRectangle(panelX, 0, panelWidth, screenHeight,
                  GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
    DrawLineEx({ static_cast<float>(panelX), 0 },
               { static_cast<float>(panelX), static_cast<float>(screenHeight) },
               2, GetColor(GuiGetStyle(DEFAULT, BORDER_COLOR_NORMAL)));

    DrawRectangle(panelX, 0, panelWidth, 40,
                  GetColor(GuiGetStyle(DEFAULT, BORDER_COLOR_NORMAL)));
    GuiDrawText("Factory Game",
                { static_cast<float>(panelX), 0, static_cast<float>(panelWidth), 40 },
                TEXT_ALIGN_CENTER, WHITE);

    int y = 55;
    const int labelH     = 18;
    const int valueH     = 18;
    const int sliderH    = 22;
    const int sectionGap = 18;
    const int itemGap    = 8;
    char buf[64];

    // // ── ELEVATION ──
    // GuiDrawText("── ELEVATION ──",
    //             { static_cast<float>(panelX), static_cast<float>(y),
    //               static_cast<float>(panelWidth), static_cast<float>(labelH) },
    //             TEXT_ALIGN_CENTER, GetColor(GuiGetStyle(DEFAULT, BORDER_COLOR_FOCUSED)));
    // y += labelH + itemGap;

    // GuiDrawText("Sea Level",
    //             { static_cast<float>(panelX + padding), static_cast<float>(y),
    //               static_cast<float>(sliderW), static_cast<float>(labelH) },
    //             TEXT_ALIGN_LEFT, WHITE);
    // snprintf(buf, sizeof(buf), "%.2f", elevationSea);
    // GuiDrawText(buf, { static_cast<float>(panelX), static_cast<float>(y),
    //                    static_cast<float>(panelWidth - padding), static_cast<float>(valueH) },
    //             TEXT_ALIGN_RIGHT, LIGHTGRAY);
    // y += labelH + 2;
    // changed |= GuiSliderBar(
    //     { static_cast<float>(panelX + padding), static_cast<float>(y),
    //       static_cast<float>(sliderW), static_cast<float>(sliderH) },
    //     "0", "1", &elevationSea, 0.0f, 1.0f);
    // y += sliderH + 4;
    // GuiDrawText(elevationSea < 0.2f ? "Molto poca acqua" :
    //             elevationSea < 0.4f ? "Oceani normali"   :
    //             elevationSea < 0.6f ? "Molti mari"       : "Quasi tutto mare",
    //             { static_cast<float>(panelX + padding), static_cast<float>(y),
    //               static_cast<float>(sliderW), 16 },
    //             TEXT_ALIGN_LEFT, GRAY);
    // y += 16 + sectionGap;

    // GuiDrawText("Plains Level",
    //             { static_cast<float>(panelX + padding), static_cast<float>(y),
    //               static_cast<float>(sliderW), static_cast<float>(labelH) },
    //             TEXT_ALIGN_LEFT, WHITE);
    // snprintf(buf, sizeof(buf), "%.2f", elevationPlains);
    // GuiDrawText(buf, { static_cast<float>(panelX), static_cast<float>(y),
    //                    static_cast<float>(panelWidth - padding), static_cast<float>(valueH) },
    //             TEXT_ALIGN_RIGHT, LIGHTGRAY);
    // y += labelH + 2;
    // changed |= GuiSliderBar(
    //     { static_cast<float>(panelX + padding), static_cast<float>(y),
    //       static_cast<float>(sliderW), static_cast<float>(sliderH) },
    //     "0", "1", &elevationPlains, 0.0f, 1.0f);
    // y += sliderH + 4;
    // GuiDrawText(elevationPlains < 0.3f ? "Poche pianure"   :
    //             elevationPlains < 0.5f ? "Pianure normali" :
    //             elevationPlains < 0.7f ? "Molte pianure"   : "Quasi tutto piano",
    //             { static_cast<float>(panelX + padding), static_cast<float>(y),
    //               static_cast<float>(sliderW), 16 },
    //             TEXT_ALIGN_LEFT, GRAY);
    // y += 16 + sectionGap;

    // // ── MOISTURE ──
    // GuiDrawText("── MOISTURE ──",
    //             { static_cast<float>(panelX), static_cast<float>(y),
    //               static_cast<float>(panelWidth), static_cast<float>(labelH) },
    //             TEXT_ALIGN_CENTER, GetColor(GuiGetStyle(DEFAULT, BORDER_COLOR_FOCUSED)));
    // y += labelH + itemGap;

    // GuiDrawText("Hills Threshold",
    //             { static_cast<float>(panelX + padding), static_cast<float>(y),
    //               static_cast<float>(sliderW), static_cast<float>(labelH) },
    //             TEXT_ALIGN_LEFT, WHITE);
    // snprintf(buf, sizeof(buf), "%.2f", moistureHills);
    // GuiDrawText(buf, { static_cast<float>(panelX), static_cast<float>(y),
    //                    static_cast<float>(panelWidth - padding), static_cast<float>(valueH) },
    //             TEXT_ALIGN_RIGHT, LIGHTGRAY);
    // y += labelH + 2;
    // changed |= GuiSliderBar(
    //     { static_cast<float>(panelX + padding), static_cast<float>(y),
    //       static_cast<float>(sliderW), static_cast<float>(sliderH) },
    //     "0", "1", &moistureHills, 0.0f, 1.0f);
    // y += sliderH + 4;
    // GuiDrawText(moistureHills < 0.3f ? "Molto arido"      :
    //             moistureHills < 0.5f ? "Umidità media"    :
    //             moistureHills < 0.7f ? "Abbastanza umido" : "Molto umido",
    //             { static_cast<float>(panelX + padding), static_cast<float>(y),
    //               static_cast<float>(sliderW), 16 },
    //             TEXT_ALIGN_LEFT, GRAY);
    // y += 16 + sectionGap * 2;

    // ── HOVERED BIOME ──────────────────────────────────────────────────────
    DrawRectangle(panelX, y - 4, panelWidth, 1,
                  GetColor(GuiGetStyle(DEFAULT, BORDER_COLOR_NORMAL)));
    y += 6;
    GuiDrawText("TILE INFO",
                { static_cast<float>(panelX), static_cast<float>(y),
                  static_cast<float>(panelWidth), static_cast<float>(labelH) },
                TEXT_ALIGN_CENTER, GetColor(GuiGetStyle(DEFAULT, BORDER_COLOR_FOCUSED)));
    y += labelH + itemGap;

    if (hoveredBiome >= 0) {
        // Quadratino colorato del bioma
        const Color bColor = biomeColors[hoveredBiome];
        DrawRectangle(panelX + padding, y, 14, 14, bColor);
        DrawRectangleLines(panelX + padding, y, 14, 14, LIGHTGRAY);

        // Nome bioma
        GuiDrawText(biomeNames[hoveredBiome],
                    { static_cast<float>(panelX + padding + 18), static_cast<float>(y),
                      static_cast<float>(sliderW - 18), 16 },
                    TEXT_ALIGN_LEFT, WHITE);
        y += 20;

        // Descrizione fissa per bioma
        const char* biomeDesc = "";
        switch (hoveredBiome) {
            case BIOME_DEEP_SEA:   biomeDesc = "Acque profonde.\nImpossibile costruire.";      break;
            case BIOME_SEA:        biomeDesc = "Mare aperto.\nNavigabile.";                     break;
            case BIOME_BEACH:      biomeDesc = "Spiaggia costiera.\nBassa efficienza.";         break;
            case BIOME_PLAINS:     biomeDesc = "Pianura fertile.\nOttima per costruire.";       break;
            case BIOME_FOREST:     biomeDesc = "Foresta densa.\nRicca di legname.";             break;
            case BIOME_DESERT:     biomeDesc = "Deserto arido.\nRisorse minerali rare.";        break;
            case BIOME_HILLS:      biomeDesc = "Colline ondulate.\nBuona estrazione.";          break;
            case BIOME_MOUNTAINS:  biomeDesc = "Montagne ripide.\nAlta efficienza mineraria.";  break;
            case BIOME_SNOW:       biomeDesc = "Vetta innevata.\nRisorse cristalline.";         break;
            default:               biomeDesc = "Sconosciuto.";                                  break;
        }
        GuiDrawText(biomeDesc,
                    { static_cast<float>(panelX + padding), static_cast<float>(y),
                      static_cast<float>(sliderW), 40 },
                    TEXT_ALIGN_LEFT, LIGHTGRAY);
        y += 44;
    } else {
        GuiDrawText("Nessun tile\nsotto il cursore.",
                    { static_cast<float>(panelX + padding), static_cast<float>(y),
                      static_cast<float>(sliderW), 32 },
                    TEXT_ALIGN_LEFT, DARKGRAY);
        y += 36;
    }

    // ── ACTIONS ──
    // y += sectionGap;
    // GuiDrawText("── ACTIONS ──",
    //             { static_cast<float>(panelX), static_cast<float>(y),
    //               static_cast<float>(panelWidth), static_cast<float>(labelH) },
    //             TEXT_ALIGN_CENTER, GetColor(GuiGetStyle(DEFAULT, BORDER_COLOR_FOCUSED)));
    // y += labelH + itemGap;

    // snprintf(buf, sizeof(buf), "Seed: %d", currentSeed);
    // GuiDrawText(buf,
    //             { static_cast<float>(panelX), static_cast<float>(y),
    //               static_cast<float>(panelWidth), 16 },
    //             TEXT_ALIGN_CENTER, GRAY);
    // y += 20;

    // if (GuiButton({ static_cast<float>(panelX + padding), static_cast<float>(y),
    //                 static_cast<float>(sliderW), 30 }, "New Seed  [R]"))
    //     RegenerateMap(true);
    // y += 38;

    // if (GuiButton({ static_cast<float>(panelX + padding), static_cast<float>(y),
    //                 static_cast<float>(sliderW), 30 }, "Reset Defaults")) {
    //     elevationSea    = 0.3f;
    //     elevationPlains = 0.55f;
    //     moistureHills   = 0.45f;
    //     changed = true;
    // }
    // y += 38;

    // GuiDrawText("[R] New seed  [Space] Refresh",
    //             { static_cast<float>(panelX), static_cast<float>(y),
    //               static_cast<float>(panelWidth), 32 },
    //             TEXT_ALIGN_CENTER, DARKGRAY);

    // if (changed)
    //     RegenerateMap(false);
}

void Gui() {
    // GuiTopBar(0.08f, 1.1f, 0.12f, 0.4f, 2.0f, 0.02f, 2.0f);
    LateralRandomSliders();
}

// ─── Render ──────────────────────────────────────────────────────────────────
void Render() {
    BeginDrawing();
    ClearBackground(GetColor(0x212121ff));
    BeginMode2D(camera);
    DrawGeneratedMap();
    EndMode2D();
    Gui();
    EndDrawing();
}

// ─── Input ───────────────────────────────────────────────────────────────────
void Input() {
    const float wheel = GetMouseWheelMove();
    if (wheel != 0.0f)
        camera.zoom = Clamp(camera.zoom + wheel * 0.1f, 0.2f, 3.0f);

    if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
        const Vector2 delta = GetMouseDelta();
        camera.target.x -= delta.x / camera.zoom;
        camera.target.y -= delta.y / camera.zoom;
    }

    if (IsKeyPressed(KEY_R))     RegenerateMap(true);
    if (IsKeyPressed(KEY_SPACE)) RegenerateMap(false);
}

// ─── Close ───────────────────────────────────────────────────────────────────
void Close() {
    if (generatedMap != nullptr) {
        for (size_t i = 0; i < MAPSIZE; ++i)
            delete[] generatedMap[i];
        delete[] generatedMap;
        generatedMap = nullptr;
    }
    CloseWindow();
}

// ─── Main ────────────────────────────────────────────────────────────────────
int main() {
    Setup("Factory");

    error = styleLoad.LoadCustomGuiStyle();
    if (error) {
        printf("ERROR: invalid setup!\n");
        CloseWindow();
        return 1;
    }

    
    RegenerateMap(true);

    while (!WindowShouldClose()) {
        const float delta = GetFrameTime();
        Update(delta);
        Render();
        Input();

        if (IsWindowResized()) {
            screenWidth   = GetRenderWidth();
            screenHeight  = GetRenderHeight();
            camera.offset = { screenWidth / 2.0f, screenHeight / 2.0f };
        }
    }

    printf("Sea: %.3f | Plains: %.3f | Hills: %.3f | Seed: %d\n",
           elevationSea, elevationPlains, moistureHills, currentSeed);

    Close();
    return 0;
}