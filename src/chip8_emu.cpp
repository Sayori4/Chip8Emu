#include <algorithm>
#include <bitset>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <random>
#include <string>

#include "chip8_emu.h"

std::random_device rd;
std::mt19937 gen(rd());

bool init_chip8(chip8_emu &chip8) {
    std::fill(chip8.ram, chip8.ram + 4096, 0);
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

    chip8.pc = 0x200; //   Set program counter to start on the chip8 entry point

    chip8.i = 0x0;

    chip8.stack_pointer = 0; // Clear the stack pointer

    chip8.stack.clear();

    chip8.romName = ""; //  ZeroOut the rom name

    chip8.sound_timer = 0;
    chip8.delay_timer = 0;

    std::fill_n(chip8.V, 16, 0);

    std::fill_n(chip8.screen, 2048, false);

    std::fill_n(chip8.keypad, 16, false);

    std::cout << "Initialized chip8\n";

    return true;
}

bool load_rom(chip8_emu &chip8, std::string romName) {
    if (chip8.romName.compare(romName) == 0) {
        std::cout << "Rom file " << romName << " is currently running\n";
        return true;
    }

    init_chip8(chip8);

    chip8.romName = romName;

    std::ifstream romFile(romName, std::ios::binary); //  Read the file in binary format
    if (!romFile) {                                   //  If romfile doesn't exist, exit this function
        std::cout << "Rom file " << romName << " couldn't load\n";
        return false;
    }

    romFile.seekg(0, std::ios::end);           //  go to the end of the file
    int romSize = romFile.tellg();             //  get the file size
    romFile.seekg(0, std::ios::beg);           //  go back to the beginning
    if (romSize > sizeof(chip8.ram) - 0x200) { //  If the size of the rom is too big, exit this function
        std::cout << "Rom file " << romName << " is too large\n";
        return false;
    }
    romFile.read((char *)&chip8.ram[0x200], romSize); //  Set Ram to the rom file

    return true;
}

bool cycle(chip8_emu &chip8, bool show_debug) {
    uint16_t opcode;

    uint8_t memory1 = chip8.ram[chip8.pc];
    uint8_t memory2 = chip8.ram[chip8.pc + 1];

    opcode = memory1 << 8 | memory2;

    chip8.pc += 2;

    //  TODO: finish all opcodes

    switch (opcode & 0xF000) {
    case 0x0000:
        switch (opcode & 0x00FF) {
        case 0x00E0:
            if (show_debug)
                std::cout << "Clear Screen" << "\n";
            std::fill_n(chip8.screen, 64 * 32, 0);
            break;
        case 0x00EE:
            if (show_debug)
                std::cout << "Break from subroutine" << "\n";
            chip8.pc = chip8.stack.back();
            chip8.stack.pop_back();
            chip8.stack_pointer--;
            break;
        default:
            if (show_debug)
                std::cout << std::hex << "Incorrect opcode: " << std::showbase << opcode << "\n";
            break;
        }
        break;
    case 0x1000:
        if (show_debug)
            std::cout << "Jump to address " << mem(opcode) << "\n";
        chip8.pc = mem(opcode);
        break;
    case 0x2000:
        if (show_debug)
            std::cout << "Exec subroutine starting at " << mem(opcode) << "\n";
        chip8.stack.push_back(chip8.pc);
        chip8.stack_pointer++;
        chip8.pc = mem(opcode);
        break;
    case 0x3000:
        if (show_debug)
            std::cout << "Skip following instruction if value of register V" << regX << " equals " << imm(opcode) << "\n";
        if (chip8.V[regX] == imm(opcode)) {
            chip8.pc += 2;
        }
        break;
    case 0x4000:
        if (show_debug)
            std::cout << "Skip following instruction if value of register V" << regX << " doesn't equal " << imm(opcode) << "\n";
        if (chip8.V[regX] != imm(opcode)) {
            chip8.pc += 2;
        }
        break;
    case 0x5000:
        if (show_debug)
            std::cout << "Skip following instruction if value of register V" << regX << " equals register V" << regY << "\n";
        if (chip8.V[regX] == chip8.V[regY]) {
            chip8.pc += 2;
        }
        break;
    case 0x6000:
        if (show_debug)
            std::cout << "Store number " << imm(opcode) << " in register V" << regX << "\n";
        chip8.V[regX] = imm(opcode);
        break;
    case 0x7000:
        if (show_debug)
            std::cout << "Add value " << imm(opcode) << " to register V" << regX << "\n";
        chip8.V[regX] += imm(opcode);
        break;
    case 0x8000:
        switch (opcode & 0x000F) {
        case 0x0000:
            if (show_debug)
                std::cout << "Store value of register V" << regY << " in register V" << regX << "\n";
            chip8.V[regX] = chip8.V[regY];
            break;
        case 0x0001:
            if (show_debug)
                std::cout << "Set register V" << regX << " to register V" << regX << " OR register V" << regY << "\n";
            chip8.V[regX] |= chip8.V[regY];
            break;
        case 0x0002:
            if (show_debug)
                std::cout << "Set register V" << regX << " to register V" << regX << " AND register V" << regY << "\n";
            chip8.V[regX] &= chip8.V[regY];
            break;
        case 0x0003:
            if (show_debug)
                std::cout << "Set register V" << regX << " to register V" << regX << " XOR register V" << regY << "\n";
            chip8.V[regX] ^= chip8.V[regY];
            break;
        case 0x0004:
            if (show_debug)
                std::cout << "Add value of register V" << regY << " to register V" << regX << "\n";
            bool change;
            (chip8.V[regX] + chip8.V[regY] > 255) ? chip8.V[0xf] = 1 : chip8.V[0xf] = 0; //  if the value is bigger than 255 set VF to 1 else set VF to 0
            chip8.V[regX] += chip8.V[regY];
            break;
        case 0x0005:
            if (show_debug)
                std::cout << "Subtract value of register V" << regY << " from register V" << regX << "\n";
            (chip8.V[regX] - chip8.V[regY] < 0) ? chip8.V[0xf] = 0 : chip8.V[0xf] = 1; //  if the value is smaller than 0 set VF to 0 else set VF to 1
            chip8.V[regX] -= chip8.V[regY];
            break;
        case 0x0006:
            if (show_debug)
                std::cout << "Store value of register V" << regY << " shifted right one bit in register V" << regX << "\n";
            chip8.V[0xf] = (chip8.V[regY] & 0x01);
            chip8.V[regX] = chip8.V[regY] >> 1;
            break;
        case 0x0007:
            if (show_debug)
                std::cout << "Set register V" << regX << " to the value of register V" << regY << " minus register V" << regX << "\n";
            (chip8.V[regY] - chip8.V[regX] < 0) ? chip8.V[0xf] = 0 : chip8.V[0xf] = 1; //  if the value is smaller than 0 set VF to 0 else set VF to 1
            chip8.V[regX] = chip8.V[regY] - chip8.V[regX];
            break;
        case 0x000E:
            if (show_debug)
                std::cout << "Store value of register V" << regY << " shifted left one bit in register V" << regX << "\n";
            std::cout << "hssddsa: " << ((chip8.V[regY] & 0x80)) << "\n";
            chip8.V[0xf] = ((chip8.V[regY] & 0x80) >> 8);
            chip8.V[regX] = chip8.V[regY] << 1;
            break;
        default:
            if (show_debug)
                std::cout << std::hex << "Incorrect opcode: " << std::showbase << opcode << "\n";
            break;
        }
        break;
    case 0x9000:
        if (show_debug)
            std::cout << "Skip the following instruction if the value of register V" << regX << " is not equal to the value of register V" << regY << "\n";
        if (chip8.V[regX] != chip8.V[regY]) {
            chip8.pc += 2;
        }
        break;
    case 0xA000:
        if (show_debug)
            std::cout << "Store memory address " << mem(opcode) << " in register I\n";
        chip8.i = mem(opcode);
        break;
    case 0xB000:
        if (show_debug)
            std::cout << "Jump to address " << mem(opcode) << " plus register V0\n";
        chip8.pc = mem(opcode) + chip8.V[0x0];
        break;
    case 0xC000: {
        if (show_debug)
            std::cout << "Set register V" << regX << " to a random number with a mask of " << imm(opcode) << "\n";
        std::uniform_int_distribution<> distr(0, 255);
        uint8_t val = distr(gen);
        std::cout << (val & imm(opcode)) << "\n";
        chip8.V[regX] = val & imm(opcode);
        break;
    }
    case 0xD000: {
        if (show_debug)
            std::cout << "Draw sprite at position V" << regX << ", V" << regY << " with " << num(opcode) << " bytes of sprite data starting at the address stored in I\n";
        //  TODO: finish the logic
        uint8_t Xpos = chip8.V[regX] & 63;
        uint8_t Ypos = chip8.V[regY] & 31;
        chip8.V[0xf] = 0;

        uint16_t position = Ypos * 64 + Xpos;

        for (int j = 0; j < num(opcode); j++) {
            uint16_t realpos = position + (j * 64);
            std::string bin = std::bitset<8>((int)chip8.ram[chip8.i + j]).to_string();
            for (int k = 0; k < bin.length(); k++) {
                if (bin[k] == '1') {
                    chip8.screen[realpos + k] = true;
                } else {
                    chip8.screen[realpos + k] = false;
                }
            }
        }
        break;
    }
    case 0xE000:
        switch (opcode & 0x00FF) {
        case 0x009E:
            if (show_debug)
                std::cout << "Skip the following instruction if the key corresponding to the hex value stored in register I is pressed";
            if (chip8.i > 0xf) {
                if (chip8.keypad[chip8.i]) {
                    chip8.pc += 2;
                }
            }
            break;
        case 0x00A1:
            if (show_debug)
                std::cout << "Skip the following instruction if the key corresponding to the hex value stored in register I is not pressed";
            if (chip8.i > 0xf) {
                if (!chip8.keypad[chip8.i]) {
                    chip8.pc += 2;
                }
            }
            break;
        default:
            if (show_debug)
                std::cout << std::hex << "Incorrect opcode: " << std::showbase << opcode << "\n";
            break;
        }
        break;
    case 0xF000:
        switch (opcode & 0x00FF) {
        case 0x0007:
            if (show_debug)
                std::cout << "Store the current value of the delay timer in register V" << regX << "\n";
            chip8.V[regX] = chip8.delay_timer;
            break;
        case 0x000A:
            if (show_debug)
                std::cout << "Wait for a keypress and store the result in register V" << regX << "\n";
            chip8.pc -= 2;
            for (int i = 0; i < 16; i++) {
                if (chip8.keypad[i] == true) {
                    chip8.V[regX] = i;
                    chip8.pc += 2;
                    break;
                }
            }
            break;
        case 0x0015:
            if (show_debug)
                std::cout << "Set the delay timer to the value in register V" << regX << "\n";
            chip8.delay_timer = chip8.V[regX];
            break;
        case 0x0018:
            if (show_debug)
                std::cout << "Set the sound timer to the value in register V" << regX << "\n";
            chip8.sound_timer = chip8.V[regX];
            break;
        case 0x001E:
            if (show_debug)
                std::cout << "Add the value stored in register V" << regX << " to register I" << "\n";
            chip8.i += chip8.V[regX];
            break;
        case 0x0029: {
            if (show_debug)
                std::cout << "Set I to the memory address of sprite data corresponding to the hex digit stored in register V" << regX << "\n";
            if (chip8.V[regX] <= 0xf) {
                chip8.i = flm + (chip8.V[regX] * 5);
                break;
            }
            std::cout << "Number stored in register V" << regX << " was over the 0-f range, the program you're trying to execute might be dangerous\n";
            break;
        }
        case 0x0033:
            if (show_debug)
                std::cout << "Store the binary-coded decimal equivalent of the value stored in register V" << regX << " at addresses I, I+1, I+2" << "\n";
            break;
        case 0x0055:
            if (show_debug)
                std::cout << "Store values of registers until register V" << regX << " in memory starting at address I, I is set to I+X+1 after" << "\n";
            break;
        case 0x0065:
            if (show_debug)
                std::cout << "Fill registers until register V" << regX << " with values stored in memory at address I, I is set to I+X+1 after" << "\n";
            break;
        default:
            if (show_debug)
                std::cout << std::hex << "Incorrect opcode: " << std::showbase << opcode << "\n";
            break;
        }
        break;
    default:
        if (show_debug)
            std::cout << std::hex << "Incorrect opcode: " << std::showbase << opcode << "\n";
        break;
    }

    if (chip8.delay_timer > 0) {
        chip8.delay_timer--;
    }
    if (chip8.sound_timer > 0) {
        // TODO: implement sound
        if (chip8.sound_timer == 1)
            std::cout << "Beep" << "\n";
        chip8.sound_timer--;
    }

    return true;
}