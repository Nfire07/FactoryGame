#include <raylib.h>
#include "../headers/generation.h"
#include "../headers/biome.h"
#include <raymath.h>

static int gSeed = 0;

void SetSeedNoise(int seed) {
    gSeed = seed;
}

static int Hash(int x, int y) {
    int h = x * 374761393
          + y * 668265263
          + gSeed * 362437;
    h = (h ^ (h >> 13)) * 1274126177;
    return h;
}

static float Grad(int hash, float x, float y) {
    switch (hash & 3) {
        case 0: return  x + y;
        case 1: return -x + y;
        case 2: return  x - y;
        default: return -x - y;
    }
}

static float Fadef(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

float GetPerlinNoise(float x, float y) {
    int x0 = (int)floorf(x);
    int y0 = (int)floorf(y);
    int x1 = x0 + 1;
    int y1 = y0 + 1;

    float sx = Fadef(x - x0);
    float sy = Fadef(y - y0);

    float n0 = Grad(Hash(x0, y0), x - x0, y - y0);
    float n1 = Grad(Hash(x1, y0), x - x1, y - y0);
    float ix0 = Lerp(n0, n1, sx);

    float n2 = Grad(Hash(x0, y1), x - x0, y - y1);
    float n3 = Grad(Hash(x1, y1), x - x1, y - y1);
    float ix1 = Lerp(n2, n3, sx);

    return Lerp(ix0, ix1, sy);
}

int** generateRandomMapFromSeed(size_t size,int seed)
{
    SetSeedNoise(seed);
    int** map = MemAlloc(size * sizeof(int*));

    float scale = 0.05f;

    for (size_t i = 0; i < size; i++) {
        map[i] = MemAlloc(size * sizeof(int));

        for (size_t j = 0; j < size; j++) {
            float elevation = GetPerlinNoise(i * scale, j * scale);
            float moisture  = GetPerlinNoise((i + 1000) * scale,
                                            (j + 1000) * scale);

            elevation = (elevation + 1.0f) * 0.5f;
            moisture  = (moisture  + 1.0f) * 0.5f;

            if (elevation < 0.30f)
                map[i][j] = BIOME_SEA;
            else if (elevation < 0.55f)
                map[i][j] = BIOME_PLAINS;
            else if (moisture < 0.45f)
                map[i][j] = BIOME_HILLS;
            else
                map[i][j] = BIOME_MOUNTAINS;

        }
    }

    return map;
}
