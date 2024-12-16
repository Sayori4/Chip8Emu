#include <cstring>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include "chip8_emu.h"

bool init_chip8(chip8_emu* chip8) {
    const uint8_t font[] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    std::memcpy(&chip8->ram[0x50], font, sizeof(font)); //  Load font into chip8 memory
    
    chip8->pc = 0x200;  //  Set program counter to start on the chip8 entry point
}

bool load_rom(chip8_emu* chip8, char romName[]) {
    if (chip8->romName == romName)
    {
        std::cout << "Rom file " << romName << " is currently running";
        return true;
    }
    

    std::ifstream romFile(romName, std::ios::binary);   //  Read the file in binary format
    if(!romFile) {      //  If romfile doesn't exist, exit this function
        std::cout << "Rom file " << romName << " couldn't load";
        return false;
    }

    romFile.seekg(0, std::ios::end);    //  go to the end of the file
    int romSize = romFile.tellg();      //  get the file size
    romFile.seekg(0, std::ios::beg);    //  go back to the beginning
    if (romSize > sizeof(chip8->ram) - 0x200) {     //  If the size of the rom is too big, exit this function
        std::cout << "Rom file " << romName << " is too large";
        return false;
    }
    romFile.read((char*) &chip8->ram[0x200], romSize);  //  Read the romfile

    return true;
}