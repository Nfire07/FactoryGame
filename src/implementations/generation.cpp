#include <raylib.h>
#include "../headers/generation.h"
#include "../headers/biome.h"
#include <raymath.h>

static int gSeed = 0;

void Generation::SetSeedNoise(const int seed) {
    gSeed = seed;
}

int Generation::Hash(const int x, const int y) {
    int h = x * 374761393 + y * 668265263 + gSeed * 362437;
    h = (h ^ (h >> 13)) * 1274126177;
    return h;
}

float Generation::Grad(const int hash, const float x, const float y) {
    switch (hash & 3) {
        case 0: return  x + y;
        case 1: return -x + y;
        case 2: return  x - y;
        default: return -x - y;
    }
}

float Generation::Fadef(const float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

float Generation::GetPerlinNoise(const float x, const float y) {
    const int x0 = static_cast<int>(floorf(x));
    const int y0 = static_cast<int>(floorf(y));
    const int x1 = x0 + 1;
    const int y1 = y0 + 1;

    const float sx = Fadef(x - x0);
    const float sy = Fadef(y - y0);

    const float n0 = Grad(Hash(x0, y0), x - x0, y - y0);
    const float n1 = Grad(Hash(x1, y0), x - x1, y - y0);
    const float ix0 = Lerp(n0, n1, sx);

    const float n2 = Grad(Hash(x0, y1), x - x0, y - y1);
    const float n3 = Grad(Hash(x1, y1), x - x1, y - y1);
    const float ix1 = Lerp(n2, n3, sx);

    return Lerp(ix0, ix1, sy);
}

int** Generation::generateRandomMapFromSeed(const size_t size, const int seed, const float elevationSea, const float elevationPlains, const float moistureHills) {
    SetSeedNoise(seed);
    int** map = new int*[size];

    const float scale = 0.05f;

    for (size_t i = 0;i < size;++i) {
        map[i] = new int[size];

        for (size_t j = 0;j < size;++j) {
            float elevation = GetPerlinNoise(i * scale, j * scale);
            float moisture  = GetPerlinNoise((i + 1000) * scale, (j + 1000) * scale);

            elevation = (elevation + 1.0f) * 0.5f;
            moisture  = (moisture  + 1.0f) * 0.5f;

            if (elevation < elevationSea)
                map[i][j] = BIOME_SEA;
            else if (elevation < elevationPlains)
                map[i][j] = BIOME_PLAINS;
            else if (moisture < moistureHills)
                map[i][j] = BIOME_HILLS;
            else
                map[i][j] = BIOME_MOUNTAINS;

        }
    }

    return map;
}