#pragma once
#include <stdio.h>

class Generation {
public:
    static void SetSeedNoise(int seed);
    static int Hash(int x, int y);
    static float Grad(int hash, float x, float y);
    static float Fadef(float t);

    static float GetPerlinNoise(float x, float y);

    static int** generateRandomMapFromSeed(size_t size,int seed, float elevationSea, float elevationPlains, float moistureHills);
};