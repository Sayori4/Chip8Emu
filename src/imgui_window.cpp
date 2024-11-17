#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_sdlrenderer2.h>

#include "imgui_emu.h"
#include "imgui_window.h"

bool mainColorPicker3(const char* label, ImVec4* color, ImGuiColorEditFlags flags = 0) {
    float col[3];
    col[0] = color->x;
    col[1] = color->y;
    col[2] = color->z;

    bool res = ImGui::ColorEdit3(label, col, flags);

    color->x = col[0];
    color->y = col[1];
    color->z = col[2];

    return res;
}

void showMainWindow(imgui_config* imgui_config, app_config* app_config) {
    ImGui::SetNextWindowBgAlpha(0.45f);
    if (!ImGui::Begin("Chip-8 Emulator", &imgui_config->showMainWindow))
    {
        ImGui::End();
        return;
    }

    ImGui::Text("Welcome to my chip-8 emulator.");
    
    mainColorPicker3("Background", &app_config->bg_color);
    // Currently unused
    mainColorPicker3("Foreground (unused)", &app_config->fg_color);

    ImGui::End();
}

