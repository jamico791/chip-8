#ifndef CPU_H
#define CPU_H

#include <cstdint>

#include "display.h"

class CPU {
public:
    CPU(int w, int h);
    CPU();
    void init(bool* f, int& w, int& h);
    int execute(int opcode);
    bool flip_pixel(int x, int y);

    int PC;       // Program Counter
    int I;        // Special 16-bit register
    int delay;     // Delay register
    int sound;     // Sound register
    int SP;        // Stack Pointer
    int* stack;   // Subroutine stack
    int* V;        // 16 8-bit General purpose registers
    int width;
    int height;
    bool* frame_buffer;
};

#endif
