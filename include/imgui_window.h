#pragma once
#include "app_info.h"
#include "chip8_emu.h"
#include "fd_emu.h"
#include "imgui_emu.h"
#include "sdl_emu.h"

void showMainWindow(imgui_config &imgui_config, app_config &app_config, sdl_stuff &sdl, chip8_emu &chip8, app_info &app_info, fd_emu &fd_emu);