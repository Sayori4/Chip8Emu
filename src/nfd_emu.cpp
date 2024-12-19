#include <iostream>
#include <nfd.h>
#include <nfd_sdl2.h>
#include "sdl_emu.h"
#include "nfd_emu.h"

int nfd_initialize() {
    if (NFD_Init() != NFD_OKAY) {
        std::cout << "ERR: NativeFileDialog failed to initialize";
        return 1;
    }

    return 0;
}

void nfd_close() {
    NFD_Quit();
}

std::string nfd_openfile(sdl_stuff* sdl) {
    nfdu8char_t* outpath;

    nfdopendialogu8args_t open_args = {};

    NFD_GetNativeWindowFromSDLWindow(sdl->window, &open_args.parentWindow);
    
    nfdu8filteritem_t filter[1] = {{ "Chip 8 emulator game", "ch8" }};

    open_args.filterList = filter;
    open_args.filterCount = 1;

    nfdresult_t res = NFD_OpenDialogU8_With(&outpath, &open_args);
    if (res == NFD_OKAY) {
        std::string fp(outpath);
        return fp;
        NFD_FreePathU8(outpath);
    }
    return "";
}