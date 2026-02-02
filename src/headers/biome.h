#pragma once
#include <raylib.h>
#include "ore.h"

/*
 * List of biomes:
 * - Plain
 * - Hills
 * - Mountain
 * - Sea
 * Multipliers:
 * - Mining efficiency
 * - Building slots (Maybe)
 * - Ore percentages
 */

enum BiomeType {
    BIOME_PLAINS = 0,
    BIOME_MOUNTAINS,
    BIOME_HILLS,
    BIOME_SEA
};

class Biome {
public:
    const char* biomeName;
    Color color;
    float miningEfficiency;
    Ore* ores;
    float* orePercentages;

    Biome();
    Biome(const Biome &biome);
    Biome(const char* biomeName, Color color, float miningEfficiency, Ore* ores, float* orePercentages);
};

extern const char* biomeNames[];
extern const Color biomeColors[];