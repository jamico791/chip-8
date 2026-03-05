#include <array>
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "display.h"

using namespace std;

Display::Display(int w, int h, int s)
    : width(w),
      height(h),
      scale(s),
      window(SDL_CreateWindow("CHIP-8", width * scale, height * scale, 0)),
      renderer(SDL_CreateRenderer(window, nullptr))
{
    SDL_LogTrace(SDL_LOG_CATEGORY_RENDER, "Display::Display(int w, int h, int s) called");
}

Display::Display()
    : width(64),
      height(32),
      scale(30),
      window(SDL_CreateWindow("CHIP-8", width * scale, height * scale, 0)),
      renderer(SDL_CreateRenderer(window, nullptr))
{
    SDL_LogTrace(SDL_LOG_CATEGORY_RENDER, "Display::Display() called");
}

// SDL_Rect* get_display_size() {
//     SDL_DisplayID primary_display = SDL_GetPrimaryDisplay();
//     SDL_Rect rect;

//     if (SDL_GetDisplayBounds(primary_display, &rect)) {
//         return &rect;
//     } else {
//         SDL_Log("Get display bounds failed");
//     }
// }

// Returns false if it fails
bool Display::init() {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        SDL_LogError(SDL_LOG_CATEGORY_RENDER,"SDL Init failed");
        return false;
    }

    return false;
}

bool Display::loop(bool& running, array<bool, 64 * 32>& frame_buffer) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        // SDL_assert(event.type == SDL_EVENT_KEY_DOWN); /* just checking key presses here... */
        if (event.key.scancode == SDL_SCANCODE_ESCAPE) {
            SDL_Event quit_event = SDL_Event();
            quit_event.type = SDL_EVENT_QUIT;
            SDL_PushEvent(&quit_event);
        } 

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

    render_frame(frame_buffer);
    return true;
}

void Display::shutdown() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool Display::fill_pixel(int x, int y) {
    const SDL_FRect rect = {scale * x, scale * y, (float)scale, (float)scale};
    SDL_RenderFillRect(renderer, &rect);
    return true;
}

bool Display::render_frame(array<bool, 64 * 32>& frame_buffer) {
    for (int i = 0; i < width * height; i++) {

        if (frame_buffer[i]) {
            int x = i % width;
            int y = i / width;
            if(!fill_pixel(x, y)) return false;
        }
    }
    return SDL_RenderPresent(renderer);
}