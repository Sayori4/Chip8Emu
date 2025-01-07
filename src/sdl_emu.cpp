#include <iostream>
#include <SDL2/SDL.h>
#include <imgui.h>

#include "sdl_emu.h"
#include "app_config.h"

int start_sdl(sdl_stuff& sdl, int scale) {
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_GAMECONTROLLER) != 0) {
        std::cout << "Could not initialize SDL2: " << SDL_GetError() << "\n";
        return 1;
    }

    #ifdef SDL_HINT_IME_SHOW_UI
        SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
    #endif

    sdl.window = SDL_CreateWindow("Chip-8 emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 64*scale, 32*scale, 0);
    if (sdl.window == NULL) {
        std::cout << "Could not start a window: " << SDL_GetError() << "\n";
        return 1;
    }

    sdl.renderer = SDL_CreateRenderer(sdl.window, -1, SDL_RENDERER_ACCELERATED);
    if (sdl.renderer == NULL) {
        std::cout << "Could not start a renderer: " << SDL_GetError() << "\n";
        return 1;
    }
    
    return 0;
}

void kill_sdl(sdl_stuff& sdl) {
    SDL_DestroyRenderer(sdl.renderer);
    SDL_DestroyWindow(sdl.window);
    SDL_Quit();
}

void set_color(sdl_stuff& sdl, ImVec4 color) {
    SDL_SetRenderDrawColor(sdl.renderer, color.x*255, color.y*255, color.z*255, SDL_ALPHA_OPAQUE);
}