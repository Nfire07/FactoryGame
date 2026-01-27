#pragma once
#include <stdio.h>

void SetSeedNoise(int seed);
static int Hash(int x, int y);
static float Grad(int hash, float x, float y);
static float Fadef(float t);
float GetPerlinNoise(float x, float y);
int** generateRandomMapFromSeed(size_t size,int seed);