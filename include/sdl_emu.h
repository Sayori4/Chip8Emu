#pragma once
#include <SDL2/SDL.h>
#include <imgui.h>

#include "app_config.h"
#include "chip8_emu.h"

struct sdl_stuff {
    SDL_Window *window;
    SDL_Renderer *renderer;
};

int start_sdl(sdl_stuff &sdl, int scale);

void kill_sdl(sdl_stuff &sdl);

bool draw_sdl(sdl_stuff &sdl, chip8_emu &chip8, app_config &app_config);

void set_color(sdl_stuff &sdl, ImVec4 color);