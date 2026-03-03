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
    // string filename = argv[1];
    // Chip8 chip8 = Chip8();

    // chip8.read_program(filename);

    // chip8.run();

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    SDL_Window* window = SDL_CreateWindow("CHIP-8", 768, 384, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

    // Set Background
    SDL_SetRenderDrawColor(renderer, 0x61, 0x86, 0xA9, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    // Set Pixel Color
    SDL_SetRenderDrawColor(renderer, 0x21, 0x29, 0x46, SDL_ALPHA_OPAQUE);

    bool running = true;

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT)
                running = false;
        }
        // SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, 12, 12);
        float x = 63;
        float y = 31;
        const SDL_FRect rect = SDL_FRect{12 * x, 12 * y, 12, 12};
        const SDL_FRect* p = &rect;
        SDL_RenderFillRect(renderer, p);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}