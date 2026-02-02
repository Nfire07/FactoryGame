#pragma once
#define RAYGUI_IMPLEMENTATION

class StyleLoad {
public:
    bool completed = false;
    bool success = false;

    StyleLoad();
    StyleLoad(bool completed, bool success);

    static int LoadCustomGuiStyle();
    static void* LoadStyleThreadFunction(void* arg);
};