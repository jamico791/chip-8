#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "display.h"

using namespace std;

Display::Display(int width, int height) {
    running = false;
    pixels = new bool[width * height];
    scale = 30;
    window = SDL_CreateWindow("CHIP-8", 64 * scale, 32 * scale, 0);
    renderer = SDL_CreateRenderer(window, nullptr);

}

Display::Display() {
    pixels = new bool[64 * 32];
}

bool get_display_size() {
    SDL_DisplayID primary_display = SDL_GetPrimaryDisplay();
    SDL_Rect rect;

    if (SDL_GetDisplayBounds(primary_display, &rect)) {
        cout << rect.h << " " << rect.w << " " << rect.x << " " << rect.y << endl;
    } else {
        SDL_Log("Get display bounds failed");
    }

    return true;
}

// Returns false if it fails
bool Display::init() {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        SDL_Log("SDL Init failed");
        return false;
    }
    get_display_size();

    return false;
}

bool Display::loop(bool& running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT)
            running = false;
    }
    // Set Background
    SDL_SetRenderDrawColor(renderer, 0x61, 0x86, 0xA9, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    // Set Pixel Color
    SDL_SetRenderDrawColor(renderer, 0x21, 0x29, 0x46, SDL_ALPHA_OPAQUE);
    float x = 63;
    float y = 31;

    toggle_pixel(0, 0);
    toggle_pixel(1, 0);
    toggle_pixel(1, 2);
    SDL_RenderPresent(renderer);
    return true;
}

bool Display::shutdown() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return true;
}

bool Display::toggle_pixel(int x, int y) {
    const SDL_FRect rect = {scale * x, scale * y, (float)scale, (float)scale};
    SDL_RenderFillRect(renderer, &rect);
    return true;
}