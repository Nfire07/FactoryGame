#include "../headers/biome.h"
#include <raylib.h>

const char* biomeNames[] = { "Plain", "Hills", "Mountains", "Sea", "Deep Sea", "Beach", "Desert", "Forest", "Snow" };
const Color biomeColors[] = { GREEN , (Color){147, 245, 66,255} , GRAY , BLUE , DARKBLUE , GOLD , ORANGE , DARKGREEN, WHITE };

Biome::Biome() {
    biomeName = "";
    color = BLACK;
    miningEfficiency = 0.0f;
    ores = nullptr;
    orePercentages = nullptr;
}

Biome::Biome(const Biome &biome) {
    biomeName = biome.biomeName;
    color = biome.color;
    miningEfficiency = biome.miningEfficiency;
    ores = biome.ores;
    orePercentages = biome.orePercentages;
}

Biome::Biome(const char* biomeName, Color color, float miningEfficiency, Ore* ores, float* orePercentages) {
    this->biomeName = biomeName;
    this->color = color;
    this->miningEfficiency = miningEfficiency;
    this->orePercentages = orePercentages;
    this->ores = ores;
}