#pragma once
#include <nfd.h>
#include <nfd_sdl2.h>
#include "sdl_emu.h"


int nfd_initialize();

void nfd_close();

std::string nfd_openfile(sdl_stuff& sdl);