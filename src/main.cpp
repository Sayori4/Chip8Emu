#include <iostream>
#include <SDL2/SDL.h>
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_sdlrenderer2.h>

#include "imgui_emu.h"
#include "sdl_emu.h"
#include "app_config.h"
#include "chip8_emu.h"
#include "nfd_emu.h"
#include "app_info.h"

int main(int, char**){
    imgui_config imgui_config;
    sdl_stuff sdl;
    app_config app_config;
    chip8_emu chip8;
    app_info app_info;

    app_config.isDebug = true;
    app_config.isPaused = true;

    app_config.scale = 20;
    app_config.bg_color = ImVec4(0,0,0,1);
    app_config.fg_color = ImVec4(1,1,1,1);

    if (start_sdl(sdl, app_config.scale) != 0) {
        return 1;
    }

    if (nfd_initialize() != 0) {
        return 1;
    }

    if (init_chip8(chip8) == false) {
        return 1;
    }
    
    if (imgui_init(sdl) != 0) {
        return 1;
    }

    imgui_config.showDemoWindow = false;
    imgui_config.showMainWindow = true;

    app_config.windowShouldClose = false;

    SDL_RenderClear(sdl.renderer);

    SDL_Event e;

    while (!app_config.windowShouldClose) {
        Uint32 startTicks = SDL_GetTicks();
        Uint64 startPerf = SDL_GetPerformanceCounter();

        while (SDL_PollEvent(&e) != 0) {
            ImGui_ImplSDL2_ProcessEvent(&e);    //  Handle imgui events
            switch (e.type)
            {
            case SDL_QUIT:
                app_config.windowShouldClose = true;
                break;
            
            case SDL_KEYDOWN:
                switch (e.key.keysym.scancode)
                {
                    case SDL_SCANCODE_F2:
                        imgui_config.showMainWindow = !imgui_config.showMainWindow;
                        break;
                    case SDL_SCANCODE_F3:
                        imgui_config.showDemoWindow = !imgui_config.showDemoWindow;
                        break;
                    default:
                        std::cout << "Unused scancode: " << e.key.keysym.scancode << "\n";
                        break;
                }

            default:
                break;
            }
            
            
        }

        if (!app_config.isPaused) {
            //Cycle the cpu
        }

        imgui_frame();

        imgui_show(imgui_config, app_config, sdl, chip8, app_info);

        ImGui::Render();
        set_color(sdl, app_config.bg_color);
        SDL_RenderClear(sdl.renderer);
        set_color(sdl, app_config.fg_color);
        draw(chip8, sdl);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), sdl.renderer);
        SDL_RenderPresent(sdl.renderer);

        Uint64 endPerf = SDL_GetPerformanceCounter();

        float elapsedTime = (endPerf - startPerf) / (float)SDL_GetPerformanceFrequency() * 1000.0f;

        if (floor(16.666f - elapsedTime) > 0) {
            SDL_Delay(floor(16.666f - elapsedTime));
        }
        
        Uint32 endTicks = SDL_GetTicks();

        app_info.frameTime = (endTicks - startTicks) / 1000.0f;
        app_info.FPS = (1.0f / app_info.frameTime); 
    }

    imgui_close();
    nfd_close();
    kill_sdl(sdl);

    return 0;
}
