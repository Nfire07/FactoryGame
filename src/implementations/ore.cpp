#include "../headers/ore.h"

const char* OreTypeToString(const OreType oreType) {
    switch (oreType) {
        case STONE:
            return "STONE";
        case COAL:
            return "COAL";
        default:
            return "UNKNOWN";
    }
}