#pragma once
#include <SDL2/SDL.h>
#include <imgui.h>

struct sdl_stuff {
    SDL_Window* window;
    SDL_Renderer* renderer;
};


int start_sdl(sdl_stuff& sdl, int scale);

void kill_sdl(sdl_stuff& sdl);

void set_color(sdl_stuff& sdl, ImVec4 color);