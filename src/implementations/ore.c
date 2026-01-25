#include "../headers/ore.h"

char* OreTypeToString(const OreType oreType) {
    switch (oreType) {
        case STONE:
            return "STONE";
        case COAL:
            return "COAL";
        default:
            return "UNKNOWN";
    }
}