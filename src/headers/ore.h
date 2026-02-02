#pragma once
#include <raylib.h>

/*
 * List of ores:
 * - Stone
 * - Coal
 */
enum OreType {
    STONE = 0,
    COAL
};

const char* OreTypeToString(OreType oreType);

class Ore {
public:
    Color color;
    OreType oreType;
};