#include <SDL3/SDL.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>
#include <imgui.h>

#include "app_config.h"
#include "app_info.h"
#include "chip8_emu.h"
#include "fd_emu.h"
#include "imgui_emu.h"
#include "imgui_window.h"
#include "sdl_emu.h"

int imgui_init(sdl_stuff &sdl) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL3_InitForSDLRenderer(sdl.window, sdl.renderer);
    ImGui_ImplSDLRenderer3_Init(sdl.renderer);

    return 0;
}

void imgui_frame() {
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
}

void imgui_show(imgui_config &imgui_config, app_config &app_config, sdl_stuff &sdl, chip8_emu &chip8, app_info &app_info, fd_emu &fd_emu) {
    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
    if (imgui_config.showDemoWindow) {
        ImGui::ShowDemoWindow(&imgui_config.showDemoWindow);
    }

    if (imgui_config.showMainWindow) {
        showMainWindow(imgui_config, app_config, sdl, chip8, app_info, fd_emu);
    }
}

void imgui_close() {
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}