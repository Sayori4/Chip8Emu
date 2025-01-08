#include <imgui.h>
#include <iostream>
#include <cstdint>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_sdlrenderer2.h>

#include "imgui_emu.h"
#include "imgui_window.h"
#include "sdl_emu.h"
#include "nfd_emu.h"
#include "chip8_emu.h"

bool mainColorPicker3(const char* label, ImVec4& color, ImGuiColorEditFlags flags = 0) {
    float col[3];
    col[0] = color.x;
    col[1] = color.y;
    col[2] = color.z;

    bool res = ImGui::ColorEdit3(label, col, flags);

    color.x = col[0];
    color.y = col[1];
    color.z = col[2];

    return res;
}

void showMainWindow(imgui_config& imgui_config, app_config& app_config, sdl_stuff& sdl, chip8_emu& chip8) {
    ImGui::SetNextWindowBgAlpha(0.45f);
    if (!ImGui::Begin("Chip-8 Emulator", &imgui_config.showMainWindow, ImGuiWindowFlags_MenuBar))
    {
        ImGui::End();
        return;
    }
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open", "Ctrl+O")) {
                app_config.gameName = nfd_openfile(sdl);
                std::cout << "Game path: " << app_config.gameName << "\n";
                load_rom(chip8, app_config.gameName);
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Quit", "Ctrl+W")) {
                app_config.windowShouldClose = true;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGui::Text("Welcome to my chip-8 emulator.");
    if (chip8.romName != "") {
        ImGui::Text("Currently running rom: %s", chip8.romName.substr(chip8.romName.find_last_of("/")+1).c_str());
    } else {
        ImGui::Text("Currently running rom: %s", "None");
    }
    
    mainColorPicker3("Background", app_config.bg_color);
    // Currently unused
    mainColorPicker3("Foreground (unused)", app_config.fg_color);

    ImGui::SeparatorText("Chip-8 Data");

    ImGui::Text("Program Counter: %i", chip8.pc);
    ImGui::Text("Index Register: %i", chip8.i);

    ImGui::SeparatorText("Chip-8 Debug");

    if (ImGui::Button("Read Next Opcode")) {
        uint16_t opcode;
        opcode = cycle(chip8);
        std::cout << std::hex << "Read Opcode: " << std::showbase << opcode << "\n";
    }

    ImGui::End();
}

