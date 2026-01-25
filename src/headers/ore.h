#pragma once
#include <raylib.h>

/*
 * List of ores:
 * - Stone
 * - Coal
 */
typedef enum OreType_e {
    STONE = 0,
    COAL
} OreType;

char* OreTypeToString(OreType oreType);

typedef struct Ore_s {
    Color color;
    OreType oreType;
} Ore;