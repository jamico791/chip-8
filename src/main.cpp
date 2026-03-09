#include <chrono>
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
using hrclock = chrono::high_resolution_clock;

int main(int argc, char *argv[]) {
    SDL_SetLogPriorities(SDL_LOG_PRIORITY_WARN);

    const double instruction_rate = 700.0;
    const double instruction_time = 1.0 / instruction_rate;
    string filename = argv[1];

    Chip8 chip8 = Chip8();
    chip8.init();
    chip8.read_program(filename);

    bool running = true;
    hrclock::time_point last = hrclock::now();

    while (running) {
        chrono::duration<double> time_span = chrono::duration_cast<chrono::duration<double>>(hrclock::now() - last);
        bool should_execute = time_span.count() >= instruction_time;
        if (should_execute)
            last = hrclock::now();
        chip8.loop(running, should_execute);
    }

    chip8.shutdown();
    return 0;
}