//////////////////////////////////////////////////////////////////////////////////
//                                                                              //
// StyleAsCode exporter v2.0 - Style data exported as a values array            //
//                                                                              //
// USAGE: On init call: GuiLoadStyleGuistyle();                                   //
//                                                                              //
// more info and bugs-report:  github.com/raysan5/raygui                        //
// feedback and support:       ray[at]raylibtech.com                            //
//                                                                              //
// Copyright (c) 2020-2025 raylib technologies (@raylibtech)                    //
//                                                                              //
//////////////////////////////////////////////////////////////////////////////////
#include "raygui.h"
#define GUISTYLE_STYLE_PROPS_COUNT  17

// Custom style name: guistyle
static const GuiStyleProp guistyleStyleProps[GUISTYLE_STYLE_PROPS_COUNT] = {
    { 0, 0, (int)0x667384ff },    // DEFAULT_BORDER_COLOR_NORMAL 
    { 0, 1, (int)0x1e1e20ff },    // DEFAULT_BASE_COLOR_NORMAL 
    { 0, 2, (int)0xb4bfd0ff },    // DEFAULT_TEXT_COLOR_NORMAL 
    { 0, 3, (int)0x4998ecff },    // DEFAULT_BORDER_COLOR_FOCUSED 
    { 0, 4, (int)0x6ca1d2ff },    // DEFAULT_BASE_COLOR_FOCUSED 
    { 0, 5, (int)0xe6e2e2ff },    // DEFAULT_TEXT_COLOR_FOCUSED 
    { 0, 6, (int)0x292727ff },    // DEFAULT_BORDER_COLOR_PRESSED 
    { 0, 7, (int)0x3662baff },    // DEFAULT_BASE_COLOR_PRESSED 
    { 0, 8, (int)0xdededeff },    // DEFAULT_TEXT_COLOR_PRESSED 
    { 0, 9, (int)0x3e4550ff },    // DEFAULT_BORDER_COLOR_DISABLED 
    { 0, 10, (int)0x2e353dff },    // DEFAULT_BASE_COLOR_DISABLED 
    { 0, 11, (int)0x484f57ff },    // DEFAULT_TEXT_COLOR_DISABLED 
    { 0, 16, (int)0x0000000b },    // DEFAULT_TEXT_SIZE 
    { 0, 17, (int)0x00000002 },    // DEFAULT_TEXT_SPACING 
    { 0, 18, (int)0x96a3b4ff },    // DEFAULT_LINE_COLOR 
    { 0, 19, (int)0x292c33ff },    // DEFAULT_BACKGROUND_COLOR 
    { 0, 20, (int)0x00000010 },    // DEFAULT_TEXT_LINE_SPACING 
};

// Style loading function: guistyle
static void GuiLoadStyleGuistyle(void)
{
    // Load style properties provided
    // NOTE: Default properties are propagated
    for (int i = 0; i < GUISTYLE_STYLE_PROPS_COUNT; i++)
    {
        GuiSetStyle(guistyleStyleProps[i].controlId, guistyleStyleProps[i].propertyId, guistyleStyleProps[i].propertyValue);
    }

    //-----------------------------------------------------------------

    // TODO: Custom user style setup: Set specific properties here (if required)
    // i.e. Controls specific BORDER_WIDTH, TEXT_PADDING, TEXT_ALIGNMENT
}
