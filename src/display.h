#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

class Display {
public:
    Display(int width, int height);    
    Display();
    bool init();
    bool loop(bool& running);
    bool shutdown();
    bool toggle_pixel(int x, int y);
private:
    bool running;
    int scale;
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool* pixels;
};

#endif