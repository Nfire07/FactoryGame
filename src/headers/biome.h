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

extern char* biomeNames[];
extern Color biomeColors[];

typedef struct Biome_s {
    char* biomeName;
    Color color;
    float miningEfficiency;
    Ore* ores;
    float* orePercentages;
} Biome;