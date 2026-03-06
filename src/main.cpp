#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include <cstdint>
#include <string>
#include <filesystem>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "chip8.h"

using namespace std;


int main(int argc, char *argv[]) {
    SDL_SetLogPriorities(SDL_LOG_PRIORITY_DEBUG);
    string filename = argv[1];
    Chip8 chip8 = Chip8();
    // chip8.step();
    chip8.init();
    chip8.read_program(filename);

    bool running = true;

    while (running) {
        chip8.loop(running);
    }

    chip8.shutdown();

    return 0;
}