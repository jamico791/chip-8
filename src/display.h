#ifndef DISPLAY_H
#define DISPLAY_H

#include <array>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

using namespace std;

class Display {
public:
    int width;
    int height;
    int scale;

    Display(int w, int h, int s);    
    Display();
    bool init();
    int loop(bool& running, array<bool, 64 * 32>& frame_buffer);
    void shutdown();
    bool fill_pixel(int x, int y);
    bool render_frame(array<bool, 64 * 32>& frame_buffer);
    SDL_Window* window;
    SDL_Renderer* renderer;
};

#endif