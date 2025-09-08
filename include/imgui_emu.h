#pragma once
#include "app_config.h"
#include "app_info.h"
#include "chip8_emu.h"
#include "fd_emu.h"
#include "sdl_emu.h"
#include <SDL3/SDL.h>

struct imgui_config {
    bool showDemoWindow;
    bool showMainWindow;
    bool enableKBInputs;
};

int imgui_init(sdl_stuff &sdl);
void imgui_frame();
void imgui_show(imgui_config &imgui_config, app_config &app_config, sdl_stuff &sdl, chip8_emu &chip8, app_info &app_info, fd_emu &fd_emu);
void imgui_close();
