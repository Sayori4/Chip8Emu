#pragma once
#include <string>
#include <imgui.h>

struct app_config {
    bool isPaused;
    bool isDebug;
    int scale;
    std::string gameName;
    bool windowShouldClose;
    ImVec4 bg_color;
    ImVec4 fg_color;
};
