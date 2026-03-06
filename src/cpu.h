#ifndef CPU_H
#define CPU_H

#include <random>
#include <cstdint>
#include <array>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "display.h"
#include "memory.h"

using namespace std;

class CPU {
public:
    int PC;                    // Program Counter
    int I;                     // Special 16-bit register
    int DT;                    // Delay timer
    int ST;                    // Sound timer
    int SP;                    // Stack Pointer
    array<int, 0x10> sub_stack;    // Subroutine stack
    array<int, 0x10> V;        // 16 8-bit General purpose registers
    int width;
    int height;
    array<bool, 64 * 32> frame_buffer;
    Memory& memory;
    array<int, SDL_SCANCODE_COUNT> scancode_to_key;
    array<int, 0x10> key_to_scancode;

    CPU(Memory& m);
    void init(bool* f, int& w, int& h);
    bool execute(int opcode);
    bool flip_pixel(int x, int y);
    int get_random();

private:
    random_device rd;
    mt19937 mt;
    uniform_int_distribution<int> dist;

    void op_Dxyn(int x, int y, int n);
};

#endif
