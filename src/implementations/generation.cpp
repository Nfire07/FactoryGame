#include <raylib.h>
#include "../headers/generation.h"
#include "../headers/biome.h"
#include <raymath.h>
#include <cmath>

static int gSeed = 0;

void Generation::SetSeedNoise(const int seed) { gSeed = seed; }

int Generation::Hash(const int x, const int y) {
    int h = x * 374761393 + y * 668265263 + gSeed * 362437;
    h = (h ^ (h >> 13)) * 1274126177;
    h = (h ^ (h >> 16));
    return h & 0x7FFFFFFF;
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
    const float sx = Fadef(x - static_cast<float>(x0));
    const float sy = Fadef(y - static_cast<float>(y0));
    const float n0 = Grad(Hash(x0, y0), x - x0, y - y0);
    const float n1 = Grad(Hash(x1, y0), x - x1, y - y0);
    const float ix0 = Lerp(n0, n1, sx);
    const float n2 = Grad(Hash(x0, y1), x - x0, y - y1);
    const float n3 = Grad(Hash(x1, y1), x - x1, y - y1);
    const float ix1 = Lerp(n2, n3, sx);
    return Lerp(ix0, ix1, sy);
}

float Generation::GetFBM(float x, float y, int octaves, float lacunarity, float gain) {
    float value     = 0.0f;
    float amplitude = 0.5f;
    float frequency = 1.0f;
    float maxValue  = 0.0f;
    for (int i = 0; i < octaves; ++i) {
        value    += GetPerlinNoise(x * frequency, y * frequency) * amplitude;
        maxValue += amplitude;
        amplitude *= gain;
        frequency *= lacunarity;
    }
    return value / maxValue;
}

static float IslandFalloff(const size_t i, const size_t j, const size_t size) {
    const float nx = static_cast<float>(i) / static_cast<float>(size - 1) * 2.0f - 1.0f;
    const float ny = static_cast<float>(j) / static_cast<float>(size - 1) * 2.0f - 1.0f;
    const float dist = sqrtf(nx * nx + ny * ny) / 1.41421356f;
    const float t = Clamp(dist, 0.0f, 1.0f);
    return 1.0f - t * t * (3.0f - 2.0f * t);
}

int** Generation::generateRandomMapFromSeed(
    const size_t size, const int seed,
    const float elevationSea, const float elevationPlains, const float moistureHills)
{
    SetSeedNoise(seed);

    int** map = new int*[size];
    for (size_t i = 0; i < size; ++i)
        map[i] = new int[size];

    const float baseScale    = 0.004f;
    const int   elevOctaves  = 6;
    const int   moistOctaves = 4;
    const float lacunarity   = 2.0f;
    const float elevGain     = 0.5f;
    const float moistGain    = 0.55f;
    const float moistOffX    = 3713.7f;
    const float moistOffY    = 1893.3f;

    // Soglie ribilanciate:
    // - highMountain alzato a +0.30f → neve solo sulle vette più estreme
    // - fascia colline più ampia come buffer tra pianure e montagne
    const float deepSeaLevel = elevationSea * 0.5f;
    const float beachLevel   = elevationSea + 0.04f;
    const float hillsLevel   = elevationPlains + 0.10f; // NUOVO: inizio colline
    const float mountLevel   = elevationPlains + 0.22f; // NUOVO: inizio montagne
    const float snowLevel    = elevationPlains + 0.38f; // alzato: neve rarissima

    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            const float fx = static_cast<float>(i) * baseScale;
            const float fy = static_cast<float>(j) * baseScale;

            float elev = GetFBM(fx, fy, elevOctaves, lacunarity, elevGain);
            elev = (elev + 1.0f) * 0.5f;
            const float falloff = IslandFalloff(i, j, size);
            elev = Clamp(elev * (0.5f + 0.5f * falloff) - (1.0f - falloff) * 0.35f,
                         0.0f, 1.0f);

            float moist = GetFBM(fx + moistOffX, fy + moistOffY,
                                 moistOctaves, lacunarity, moistGain);
            moist = (moist + 1.0f) * 0.5f;

            BiomeType biome;

            if (elev < deepSeaLevel) {
                biome = BIOME_DEEP_SEA;
            }
            else if (elev < elevationSea) {
                biome = BIOME_SEA;
            }
            else if (elev < beachLevel) {
                biome = BIOME_BEACH;
            }
            else if (elev < elevationPlains) {
                // Zona bassa: pianure/foreste/deserti
                if (moist < 0.20f)
                    biome = BIOME_DESERT;
                else if (moist < 0.55f)
                    biome = BIOME_PLAINS;
                else
                    biome = BIOME_FOREST;
            }
            else if (elev < hillsLevel) {
                // 1
                // (transizione graduale da pianura)
                biome = BIOME_MOUNTAINS;
            }
            else if (elev < mountLevel) {
                // Colline o montagne in base all'umidità
                if (moist < moistureHills)
                    // 2
                    biome = BIOME_MOUNTAINS;
                else
                    biome = BIOME_HILLS;
            }
            else if (elev < snowLevel) {
                // Alta quota: quasi sempre montagne
                biome = BIOME_SNOW;
            }
            else {
                // Neve solo sopra snowLevel (molto rara)
                biome = BIOME_SNOW;
            }

            map[i][j] = static_cast<int>(biome);
        }
    }

    return map;
}