#pragma once
#include <stdbool.h>

typedef struct {
    bool completed;
    bool success;
} StyleLoadStatus;

int LoadCustomGuiStyle();
void* LoadStyleThreadFunction(void* arg);