#include <SDL2/SDL.h>
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_sdlrenderer2.h>

#include "imgui_emu.h"
#include "imgui_window.h"
#include "sdl_emu.h"
#include "app_config.h"

int imgui_init(sdl_stuff* sdl) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForSDLRenderer(sdl->window, sdl->renderer);
    ImGui_ImplSDLRenderer2_Init(sdl->renderer);

    return 0;
}

void imgui_frame() {
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void imgui_show(imgui_config* imgui_config, app_config* app_config) {
    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
    if (imgui_config->showDemoWindow) {
        ImGui::ShowDemoWindow(&imgui_config->showDemoWindow);
    }
    //  TODO: Add showing other windows

    if (imgui_config->showMainWindow) {
        showMainWindow(imgui_config, app_config);
    }
    
}

void imgui_close() {
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}