#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

class Display {
public:
    int width;
    int height;
    int scale;
    bool* frame_buffer;

    Display(int w, int h, int s, bool* f);    
    Display();
    bool init();
    bool loop(bool& running);
    void shutdown();
    bool fill_pixel(int x, int y);
    bool render_frame();
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
};

#endif