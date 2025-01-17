#include <cstring>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>

#include "chip8_emu.h"
#include "sdl_emu.h"

bool init_chip8(chip8_emu& chip8) {
    std::fill(chip8.ram, chip8.ram+4096, 0);
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

    std::memcpy(&chip8.ram[0x50], font, sizeof(font)); //  Load font into chip8 memory
    
    chip8.pc = 0x200;  //   Set program counter to start on the chip8 entry point

    chip8.i = 0x0;
    
    chip8.stack_pointer = 0; // Clear the stack pointer

    chip8.stack.clear();

    chip8.romName = ""; //  ZeroOut the rom name

    std::fill_n(chip8.V, 15, 0);

    std::cout << "Initialized chip8\n";

    return true;
}

bool load_rom(chip8_emu& chip8, std::string romName) {
    if (chip8.romName.compare(romName) == 0)
    {
        std::cout << "Rom file " << romName << " is currently running\n";
        return true;
    }

    init_chip8(chip8);

    chip8.romName = romName;
    
    std::ifstream romFile(romName, std::ios::binary);   //  Read the file in binary format
    if(!romFile) {      //  If romfile doesn't exist, exit this function
        std::cout << "Rom file " << romName << " couldn't load\n";
        return false;
    }

    romFile.seekg(0, std::ios::end);    //  go to the end of the file
    int romSize = romFile.tellg();      //  get the file size
    romFile.seekg(0, std::ios::beg);    //  go back to the beginning
    if (romSize > sizeof(chip8.ram) - 0x200) {     //  If the size of the rom is too big, exit this function
        std::cout << "Rom file " << romName << " is too large\n";
        return false;
    }
    romFile.read((char*) &chip8.ram[0x200], romSize);  //  Set Ram to the rom file

    return true;
}

bool draw(chip8_emu& chip8, sdl_stuff& sdl) {
    // chip8.draw_flag = false;
    return true;
}

u_int16_t cycle(chip8_emu& chip8) {
    uint16_t opcode;

    uint8_t memory1 = chip8.ram[chip8.pc];
    uint8_t memory2 = chip8.ram[chip8.pc+1];

    opcode = memory1 << 8 | memory2;

    chip8.pc += 2;

    switch (opcode & 0xF000) {
        case 0x0000:
            switch (opcode & 0x00FF) {
                case 0x00E0:
                    std::cout << "Clear Screen" << "\n";
                    break;
                case 0x00EE:
                    std::cout << "Break from subroutine" << "\n";
                    break;
                default:
                    std::cout << std::hex << "Incorrect opcode: " << std::showbase << opcode << "\n";
                    break;
            }
            break;
        case 0x1000:
            std::cout << "Jump to address " << (opcode & 0x0FFF) << "\n";
            break;
        case 0x2000:
            std::cout << "Exec subroutine starting at " << (opcode & 0x0FFF) << "\n";
            break;
        case 0x3000:
            std::cout << "Skip following instruction if value of register V" << (opcode & 0x0F00) << " equals " << (opcode & 0x00FF) << "\n";
            break;
        case 0x4000:
            std::cout << "Skip following instruction if value of register V" << (opcode & 0x0F00) << " equals " << (opcode & 0x00FF) << "\n";
            break;
        case 0x5000:
            std::cout << "Skip following instruction if value of register V" << (opcode & 0x0F00) << " equals register V" << (opcode & 0x00F0) << "\n";
            break;
        case 0x6000:
            std::cout << "Store number " << (opcode & 0x00FF) << " in register V" << (opcode & 0x0F00) << "\n";
            break;
        case 0x7000:
            std::cout << "Add value " << (opcode & 0x00FF) << " to register V" << (opcode & 0x0F00) << "\n";
            break;
        case 0x8000:
            switch (opcode & 0x000F) {
            case 0x0000:
                std::cout << "Store value of register V" << (opcode & 0x00F0) << " in register V" << (opcode & 0x0F00) << "\n";
                break;
            case 0x0001:
                std::cout << "Set register V" << (opcode & 0x0F00) << " to register V" << (opcode & 0x0F00) << " OR register V" << (opcode & 0x00F0) << "\n";
                break;
            case 0x0002:
                std::cout << "Set register V" << (opcode & 0x0F00) << " to register V" << (opcode & 0x0F00) << " AND register V" << (opcode & 0x00F0) << "\n";
                break;
            case 0x0003:
                std::cout << "Set register V" << (opcode & 0x0F00) << " to register V" << (opcode & 0x0F00) << " XOR register V" << (opcode & 0x00F0) << "\n";
                break;
            case 0x0004:
                std::cout << "Add value of register V" << (opcode & 0x00F0) << " to register V" << (opcode & 0x0F00) << "\n";
                break;
            case 0x0005:
                std::cout << "Subtract value of register V" << (opcode & 0x00F0) << " to register V" << (opcode & 0x0F00) << "\n";
                break;
            case 0x0006:
                std::cout << "Store value of register V" << (opcode & 0x00F0) << " shifted right one bit in register V" << (opcode & 0x0F00) << "\n";
                break;
            // case 0x0007:
            //     std::cout << 
            default:
                std::cout << std::hex << "Incorrect opcode: " << std::showbase << opcode << "\n";
                break;
            }
            break;
        default:
            std::cout << std::hex << "Incorrect opcode: " << std::showbase << opcode << "\n";
            break;
    }

    return opcode;
}