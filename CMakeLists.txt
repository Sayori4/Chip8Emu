cmake_minimum_required(VERSION 3.20.0)
project(chip_8_emulator VERSION 0.1.0 LANGUAGES C CXX)

add_subdirectory(lib/imgui)
add_subdirectory(lib/SDL)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(chip_8_emulator 
    "src/main.cpp"
    "src/imgui_emu.cpp"
    "src/imgui_window.cpp"
    "src/sdl_emu.cpp"
    "src/chip8_emu.cpp"
    "src/fd_emu.cpp"
)

target_include_directories(chip_8_emulator PRIVATE include)

target_link_libraries(chip_8_emulator SDL3::SDL3 imgui)