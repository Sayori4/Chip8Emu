#pragma once
#include <cstdint>
#include <string>
#include <vector>

#include "sdl_emu.h"

struct chip8_emu{
    uint8_t ram[4096];              //  Originally the chip-8 interpreter uses 4mb of ram
    bool screen[64*32];             //  Here we don't need to worry about the scale since it's just emulation
    uint16_t pc;                    //  Program counter (points at current memory instruction)
    uint16_t i;                     //  Index Register (Point at locations in memory)
    std::vector<uint16_t> stack;    //  Stack
    uint8_t stack_pointer;          //  Stack Pointer
    uint8_t delay_timer;            //  Delay Timer (decrements at 60hz until it reaches 0)
    uint8_t sound_timer;            //  Sound Timer (same as delay timer but beeps as long as it's not equal to 0)
    uint8_t V[16];                  //  Data Registers (0-F)
    bool draw_flag;                 //  Draw flag
    std::string romName;            //  Only here so we can check if currently loaded rom is the same as the trying to load one, don't add anything below this.
};

bool init_chip8(chip8_emu& chip8);
bool load_rom(chip8_emu& chip8, std::string rom_name);
bool cycle(chip8_emu& chip8);
bool draw(chip8_emu& chip8, sdl_stuff& sdl);