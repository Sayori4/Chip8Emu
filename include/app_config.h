#pragma once
#include <string>
#include <imgui.h>

struct app_config {
    std::string gameName;
    bool windowShouldClose;
    ImVec4 bg_color;
    ImVec4 fg_color;
};
