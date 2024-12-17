#pragma once
#include <SDL2/SDL.h>
#include "app_config.h"

struct sdl_stuff {
    SDL_Window* window;
    SDL_Renderer* renderer;
};


int start_app(sdl_stuff* sdl, int scale);

void kill_app(sdl_stuff* sdl);

void set_bg_color(sdl_stuff* sdl, app_config* app_config);