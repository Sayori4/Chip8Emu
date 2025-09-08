#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_scancode.h>
#include <cmath>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include <iostream>

#include "app_config.h"
#include "app_info.h"
#include "chip8_emu.h"
#include "fd_emu.h"
#include "imgui_emu.h"
#include "sdl_emu.h"

int main(int argc, char *argv[]) {
    imgui_config imgui_config;
    sdl_stuff sdl;
    chip8_emu chip8;
    app_config app_config;
    app_info app_info;

    app_config.isDebug = true;
    app_config.isPaused = true;

    app_config.scale = 20;
    app_config.bg_color = ImVec4(0, 0, 0, 1);
    app_config.fg_color = ImVec4(1, 1, 1, 1);

    if (start_sdl(sdl, app_config.scale) != 0) {
        return 1;
    }

    if (init_chip8(chip8) == false) {
        return 1;
    }

    if (imgui_init(sdl) != 0) {
        return 1;
    }

    fd_emu fd_emu(&chip8, &sdl);

    imgui_config.showDemoWindow = false;
    imgui_config.showMainWindow = true;

    app_config.windowShouldClose = false;

    SDL_RenderClear(sdl.renderer);

    SDL_Event e;

    while (!app_config.windowShouldClose) {
        Uint32 startTicks = SDL_GetTicks();
        Uint64 startPerf = SDL_GetPerformanceCounter();

        while (SDL_PollEvent(&e) != 0) {
            ImGui_ImplSDL3_ProcessEvent(&e); //  Handle imgui events
            if (e.key.repeat == false) {
                switch (e.type) {
                case SDL_EVENT_QUIT:
                    app_config.windowShouldClose = true;
                    break;

                case SDL_EVENT_KEY_DOWN:
                    switch (e.key.scancode) {
                    case SDL_SCANCODE_F2:
                        imgui_config.showMainWindow = !imgui_config.showMainWindow;
                        break;
                    case SDL_SCANCODE_F3:
                        imgui_config.showDemoWindow = !imgui_config.showDemoWindow;
                        break;
                    case SDL_SCANCODE_1:
                        chip8.keypad[0x1] = true;
                        break;
                    case SDL_SCANCODE_2:
                        chip8.keypad[0x2] = true;
                        break;
                    case SDL_SCANCODE_3:
                        chip8.keypad[0x3] = true;
                        break;
                    case SDL_SCANCODE_4:
                        chip8.keypad[0xc] = true;
                        break;
                    case SDL_SCANCODE_Q:
                        chip8.keypad[0x4] = true;
                        break;
                    case SDL_SCANCODE_W:
                        chip8.keypad[0x5] = true;
                        break;
                    case SDL_SCANCODE_E:
                        chip8.keypad[0x6] = true;
                        break;
                    case SDL_SCANCODE_R:
                        chip8.keypad[0xd] = true;
                        break;
                    case SDL_SCANCODE_A:
                        chip8.keypad[0x7] = true;
                        break;
                    case SDL_SCANCODE_S:
                        chip8.keypad[0x8] = true;
                        break;
                    case SDL_SCANCODE_D:
                        chip8.keypad[0x9] = true;
                        break;
                    case SDL_SCANCODE_F:
                        chip8.keypad[0xe] = true;
                        break;
                    case SDL_SCANCODE_Z:
                        chip8.keypad[0xa] = true;
                        break;
                    case SDL_SCANCODE_X:
                        chip8.keypad[0x0] = true;
                        break;
                    case SDL_SCANCODE_C:
                        chip8.keypad[0xb] = true;
                        break;
                    case SDL_SCANCODE_V:
                        chip8.keypad[0xf] = true;
                        break;
                    default:
                        if (app_config.isDebug)
                            std::cout << "Unused KeyDown scancode: " << e.key.scancode << "\n";
                        break;
                    }
                    break;
                case SDL_EVENT_KEY_UP:
                    switch (e.key.scancode) {
                    case SDL_SCANCODE_1:
                        chip8.keypad[0x1] = false;
                        break;
                    case SDL_SCANCODE_2:
                        chip8.keypad[0x2] = false;
                        break;
                    case SDL_SCANCODE_3:
                        chip8.keypad[0x3] = false;
                        break;
                    case SDL_SCANCODE_4:
                        chip8.keypad[0xc] = false;
                        break;
                    case SDL_SCANCODE_Q:
                        chip8.keypad[0x4] = false;
                        break;
                    case SDL_SCANCODE_W:
                        chip8.keypad[0x5] = false;
                        break;
                    case SDL_SCANCODE_E:
                        chip8.keypad[0x6] = false;
                        break;
                    case SDL_SCANCODE_R:
                        chip8.keypad[0xd] = false;
                        break;
                    case SDL_SCANCODE_A:
                        chip8.keypad[0x7] = false;
                        break;
                    case SDL_SCANCODE_S:
                        chip8.keypad[0x8] = false;
                        break;
                    case SDL_SCANCODE_D:
                        chip8.keypad[0x9] = false;
                        break;
                    case SDL_SCANCODE_F:
                        chip8.keypad[0xe] = false;
                        break;
                    case SDL_SCANCODE_Z:
                        chip8.keypad[0xa] = false;
                        break;
                    case SDL_SCANCODE_X:
                        chip8.keypad[0x0] = false;
                        break;
                    case SDL_SCANCODE_C:
                        chip8.keypad[0xb] = false;
                        break;
                    case SDL_SCANCODE_V:
                        chip8.keypad[0xf] = false;
                        break;
                    default:
                        if (app_config.isDebug)
                            std::cout << "Unused KeyUp scancode: " << e.key.scancode << "\n";
                        break;
                    }
                    break;
                default:
                    break;
                }
            }
        }

        if (!app_config.isPaused && chip8.romName != "") {
            cycle(chip8, app_config.isDebug); //  Cycle The CPU
        }

        imgui_frame();

        imgui_show(imgui_config, app_config, sdl, chip8, app_info, fd_emu); //  show the UI created with ImGui

        fd_emu.send_to_emu();

        ImGui::Render();
        set_color(sdl, app_config.bg_color); //  Set the background color
        SDL_RenderClear(sdl.renderer);       //  Clear the screen
        set_color(sdl, app_config.fg_color); //  Set the frontground color
        draw_sdl(sdl, chip8, app_config);    //  Draw call (Draw onto the screen)
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), sdl.renderer);
        SDL_RenderPresent(sdl.renderer);

        Uint64 endPerf = SDL_GetPerformanceCounter();

        float elapsedTime = (endPerf - startPerf) / (float)SDL_GetPerformanceFrequency() * 1000.0f;

        if (std::floor(16.666f - elapsedTime) > 0) {
            SDL_Delay(std::floor(16.666f - elapsedTime));
        }

        Uint32 endTicks = SDL_GetTicks();

        app_info.frameTime = (endTicks - startTicks) / 1000.0f;
        app_info.FPS = (1.0f / app_info.frameTime);
    }

    imgui_close();
    kill_sdl(sdl);

    return 0;
}
