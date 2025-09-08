#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>
#include <cstdint>
#include <imgui.h>

#include "chip8_emu.h"
#include "fd_emu.h"
#include "imgui_emu.h"
#include "imgui_window.h"
#include "sdl_emu.h"

bool mainColorPicker3(const char *label, ImVec4 &color, ImGuiColorEditFlags flags = 0) {
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

void showMainWindow(imgui_config &imgui_config, app_config &app_config, sdl_stuff &sdl, chip8_emu &chip8, app_info &app_info, fd_emu &fd_emu) {
    ImGui::SetNextWindowBgAlpha(0.45f);
    if (!ImGui::Begin("Chip-8 Emulator", &imgui_config.showMainWindow, ImGuiWindowFlags_MenuBar)) {
        ImGui::End();
        return;
    }
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open", "Ctrl+O")) {
                fd_emu.nfd_openfile();
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

    ImGui::Spacing();
    ImGui::SeparatorText("Emulator Info");

    if (chip8.romName != "") {
        ImGui::Text("Currently running rom: %s", chip8.romName.substr(chip8.romName.find_last_of("/") + 1).c_str());
    } else {
        ImGui::Text("Currently running rom: %s", "None");
    }

    ImGui::Spacing();

    ImGui::Text("Frame time: %f", app_info.frameTime);
    ImGui::Text("FPS: %f", app_info.FPS);

    ImGui::Separator();

    ImGui::Text("Current State: %s", (app_config.isPaused) ? "Paused" : "Playing");

    ImGui::SeparatorText("Emulator Config");

    mainColorPicker3("Background", app_config.bg_color);
    mainColorPicker3("Foreground", app_config.fg_color);

    ImGui::Spacing();

    ImGui::Checkbox("Is paused", &app_config.isPaused);

    ImGui::SeparatorText("Chip-8 Data");

    ImGui::Text("Program Counter: %i", chip8.pc);
    ImGui::Text("Index Register: %i", chip8.i);
    ImGui::Text("Stack Pointer: %i", chip8.stack_pointer);

    ImGui::Separator();

    if (ImGui::TreeNode("Stack")) {
        ImGui::BeginListBox("");
        for (uint16_t stack_num : chip8.stack) {
            ImGui::Selectable(std::to_string(stack_num).c_str());
        }
        ImGui::EndListBox();

        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Registers")) {
        if (ImGui::BeginTable("Table", 2, ImGuiTableFlags_BordersInnerH)) {
            int counter = 0;
            for (uint8_t val : chip8.V) {
                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Register %d", counter++);

                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%d", val);
            }
            ImGui::EndTable();
        }

        ImGui::TreePop();
    }

    if (app_config.isDebug) {
        ImGui::SeparatorText("Chip-8 Debug");

        if (ImGui::Button("Restart Machine")) {
            init_chip8(chip8);
        }
        ImGui::SameLine();
        if (ImGui::Button("Manual Clock")) {
            cycle(chip8, true);
        }
        ImGui::SameLine();
        if (ImGui::Button("Reset Program Counter")) {
            chip8.pc = 0x200;
        }
        if (ImGui::Button("Clear Registers")) {
            std::fill_n(chip8.V, 16, 0);
        }
    }

    ImGui::End();
}
