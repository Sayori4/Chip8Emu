#pragma once
#include <SDL2/SDL.h>
#include "sdl_emu.h"
#include "app_config.h"

struct imgui_config {
    bool showDemoWindow;
    bool showMainWindow;
    bool enableKBInputs;
};

int imgui_init(sdl_stuff* sdl);
void imgui_frame();
void imgui_show(imgui_config* imgui_config, app_config* app_config);
void imgui_close();
