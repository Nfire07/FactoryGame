#include "../headers/biome.h"
#include <raylib.h>

const char* biomeNames[] = { "Plain", "Hills", "Mountains", "Sea" };
const Color biomeColors[] = { GREEN, YELLOW, GRAY, BLUE };

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