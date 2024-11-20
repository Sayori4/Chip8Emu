#include <iostream>
#include <SDL2/SDL.h>
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_sdlrenderer2.h>

#include "imgui_emu.h"
#include "sdl_emu.h"
#include "app_config.h"

int main(int, char**){
    int scale = 20;
    SDL_Window* window;
    SDL_Renderer* renderer;
    imgui_config imgui_config;
    sdl_stuff sdl;
    app_config app_config;
    

    if (start_app(&sdl, scale) != 0) {
        return 1;
    }

    app_config.bg_color = ImVec4(0,0,0,1);
    app_config.fg_color = ImVec4(1,1,1,1);
    

    if (imgui_init(&sdl) != 0) {
        return 1;
    }

    imgui_config.showDemoWindow = false;
    imgui_config.showMainWindow = true;

    app_config.windowShouldClose = false;

    SDL_RenderClear(sdl.renderer);

    SDL_Event e;

    while (!app_config.windowShouldClose) {
        while (SDL_PollEvent(&e) != 0) {
            ImGui_ImplSDL2_ProcessEvent(&e);
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
                
                default:
                    break;
                }

            default:
                break;
            }
            
            
        }

        imgui_frame();

        imgui_show(&imgui_config, &app_config);

        ImGui::Render();
        set_bg_color(&sdl, &app_config);
        SDL_RenderClear(sdl.renderer);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), sdl.renderer);
        SDL_RenderPresent(sdl.renderer);
    }

    imgui_close();
    kill_app(&sdl);

    return 0;
}
