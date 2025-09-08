#include "fd_emu.h"
#include "SDL3/SDL_dialog.h"
#include "SDL3/SDL_filesystem.h"
#include "SDL3/SDL_stdinc.h"
#include "chip8_emu.h"
#include "sdl_emu.h"
#include <iostream>
#include <string>

static const SDL_DialogFileFilter filters[] = {
    {"Chip8 roms", "ch8"},
    {"All files", "*"}};

fd_emu::fd_emu(chip8_emu *chip8, sdl_stuff *sdl) {
    this->chip8 = chip8;
    this->sdl = sdl;
    this->userdata = ud();
    this->userdata.finished = false;
    this->userdata.data = "";
}

void fd_emu::nfd_openfile() {
    SDL_ShowOpenFileDialog(this->callback, &this->userdata, this->sdl->window, filters, SDL_arraysize(filters), SDL_GetCurrentDirectory(), false);
    std::cout << this->userdata.data << "\n";
}

void fd_emu::send_to_emu() {
    if (userdata.finished) {
        load_rom(*chip8, this->userdata.data);
        userdata.finished = false;
    }
}