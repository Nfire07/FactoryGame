#pragma once
#include <stdbool.h>
#define RAYGUI_IMPLEMENTATION

typedef struct {
    bool completed;
    bool success;
} StyleLoadStatus;

int LoadCustomGuiStyle();
void* LoadStyleThreadFunction(void* arg);