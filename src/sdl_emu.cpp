#include <SDL3/SDL.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <imgui.h>
#include <iostream>

#include "SDL3/SDL_video.h"
#include "app_config.h"
#include "chip8_emu.h"
#include "sdl_emu.h"

int start_sdl(sdl_stuff &sdl, int scale) {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
        std::cout << "Could not initialize SDL2: " << SDL_GetError() << "\n";
        return 1;
    }

    sdl.window = SDL_CreateWindow("Chip-8 emulator", 64 * scale, 32 * scale, 0);
    if (sdl.window == nullptr) {
        std::cout << "Could not start a window: " << SDL_GetError() << "\n";
        return 1;
    }

    sdl.renderer = SDL_CreateRenderer(sdl.window, nullptr);
    if (sdl.renderer == nullptr) {
        std::cout << "Could not start a renderer: " << SDL_GetError() << "\n";
        return 1;
    }

    return 0;
}

bool draw_sdl(sdl_stuff &sdl, chip8_emu &chip8, app_config &app_config) {
    SDL_FRect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = app_config.scale;
    rect.h = app_config.scale;

    for (int i = 0; i < sizeof(chip8.screen); i++) {
        rect.x = (i % 64) * app_config.scale;
        rect.y = (i / 64) * app_config.scale;

        if (chip8.screen[i]) {
            set_color(sdl, app_config.fg_color);
            SDL_RenderFillRect(sdl.renderer, &rect);
        } else {
            set_color(sdl, app_config.bg_color);
            SDL_RenderFillRect(sdl.renderer, &rect);
        }
    }
    return true;
}

void kill_sdl(sdl_stuff &sdl) {
    SDL_DestroyRenderer(sdl.renderer);
    SDL_DestroyWindow(sdl.window);
    SDL_Quit();
}

void set_color(sdl_stuff &sdl, ImVec4 color) {
    SDL_SetRenderDrawColor(sdl.renderer, color.x * 255, color.y * 255, color.z * 255, SDL_ALPHA_OPAQUE);
}