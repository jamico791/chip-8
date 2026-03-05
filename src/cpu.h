#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include <array>

#include "display.h"
#include "memory.h"

using namespace std;

class CPU {
public:
    CPU(int w, int h, Memory& m);
    CPU();
    void init(bool* f, int& w, int& h);
    bool execute(int opcode);
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
    array<bool, 64 * 32> frame_buffer;
    Memory memory;
};

#endif
