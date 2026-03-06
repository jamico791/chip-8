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

// /* returns 1 if moving forward with this keypress, -1 if moving backward, 0 if not moving. */
// int direction_user_should_move()
// {
//     const bool *key_states = SDL_GetKeyboardState(nullptr);
//     int direction = 0;

//     /* (We're writing our code such that it sees both keys are pressed and cancels each other out!) */
//     if (key_states[SDL_SCANCODE_W]) {
//         direction += 1;  /* pressed what would be "W" on a US QWERTY keyboard. Move forward! */
//     } 

//     if (key_states[SDL_SCANCODE_S]) {
//         direction += -1;  /* pressed what would be "S" on a US QWERTY keyboard. Move backward! */
//     }

//     /* (In practice it's likely you'd be doing full directional input in here, but for simplicity, we're just showing forward and backward) */

//     return direction;  /* wasn't key in W or S location, don't move. */
// }

int main(int argc, char *argv[]) {
    SDL_SetLogPriorities(SDL_LOG_PRIORITY_DEBUG);
    string filename = argv[1];
    Chip8 chip8 = Chip8();
    chip8.step();
    chip8.init();
    chip8.read_program(filename);

    bool running = true;

    while (running) {
        chip8.loop(running);
    }

    chip8.shutdown();

    return 0;
}