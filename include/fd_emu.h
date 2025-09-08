#pragma once
#include "chip8_emu.h"
#include "sdl_emu.h"
#include <string>

struct ud {
    bool finished;
    std::string data;
};

// void nfd_openfile(sdl_stuff &sdl);
class fd_emu {
  public:
    fd_emu(chip8_emu *chip8, sdl_stuff *sdl);
    void nfd_openfile();
    void send_to_emu();

  private:
    chip8_emu *chip8;
    sdl_stuff *sdl;
    ud userdata;
    static void SDLCALL callback(void *userdata, const char *const *filelist, int filter) {
        if (!filelist) {
            SDL_Log("An error occured: %s", SDL_GetError());
            return;
        } else if (!*filelist) {
            SDL_Log("The user did not select any file.");
            SDL_Log("Most likely, the dialog was canceled.");
            return;
        }
        // init_chip8(*chip8);
        SDL_Log("Full path to selected file: '%s'", *filelist);

        ud *data = static_cast<ud *>(userdata);

        std::string file = *filelist;
        data->data = file;
        data->finished = true;
    }
};