#include <iostream>
#include <cstdint>
#include <stdexcept>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "cpu.h"
#include "memory.h"

using namespace std;

CPU::CPU(Memory& m) 
    : PC(0x200),
      I(0),
      delay(0),
      sound(0),
      SP(0),
      sub_stack(),
      V(),
      width(64),
      height(32),
      frame_buffer(),
      memory(m)
{
}

int reverse(int b) {
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
}

bool CPU::execute(int opcode) {
    int id = opcode >> 12;
    int x = (opcode & 0xF00) >> 8;
    int y = (opcode & 0xF0) >> 4;
    int n = opcode & 0xF;
    int kk = opcode & 0xFF;
    int nnn = opcode & 0xFFF;

    switch (id) {
    case 0x0:
        if (nnn == 0x0E0) return 1;       // CLS  TODO: Implement clear display logic
        else if (nnn == 0x0EE) {
            if (SP - 1 == 0x10) throw std::out_of_range("CPU::execute(int opcode) stack underflow");
            PC = sub_stack[--SP];
            sub_stack[SP] = 0;
        }
        else if (nnn == 0x0FD) return 0; // EXIT opcode from Super Chip-48
        break;
    case 0x1:
        PC = nnn - 2;
        break;
    case 0x2:
        if (SP + 1 == 0x10) throw std::out_of_range("CPU::execute(int opcode) stack overflow");
        sub_stack[SP++] = PC;
        PC = nnn - 2;
        break;
    case 0x3:
        if (V[x] == kk) PC += 2;
        break;
    case 0x4:
        if (V[x] != kk) PC += 2;
        break;
    case 0x5:
        if (V[x] == V[y]) PC += 2;
        break;
    case 0x6:
        V[x] = kk;
        break;
    case 0x7:
        V[x] += kk;
        break;
    case 0x8:
        if      (n == 0x0) V[x] = V[y];
        else if (n == 0x1) V[x] |= V[y];
        else if (n == 0x2) V[x] &= V[y];
        else if (n == 0x3) V[x] ^= V[y];
        else if (n == 0x4) {
            int sum = V[x] + V[y];
            V[0xF] = sum > 0xFF;
            V[x] = sum & 0xFF;
        } else if (n == 0x5) {
            V[0xF] = V[x] > V[y];
            V[x] = (V[x] - V[y]) & 0xFF;
        } else if (n == 0x6) {
            V[0xF] = (V[x] & 1) == 1;
            V[x] /= 2;
        } else if (n == 0x7) {
            V[0xF] = V[y] > V[x];
            V[x] = (V[y] - V[x]) & 0xFF;
        } else if (n == 0xE) {
            V[0xF] = (V[x] & 1) == 1;
            V[x] *= 2;
        }
        break;
    case 0x9:
        if (V[x] != V[y]) PC += 2;
        break;
    case 0xA:
        I = nnn;
        break;
    case 0xB:
        PC = (nnn + V[0]) - 2;
        break;
    case 0xD:
        for (int i = I; i < I + n; i++) {
            int byte = memory[i];
            byte = reverse(byte);
            
            for (int j = 0; j < 8; j++) {
                bool bit = (byte >> j) & 1;
                SDL_LogTrace(SDL_LOG_CATEGORY_APPLICATION, "bit: : %d i: %d j: %d", bit, i, j);
                if (bit) {
                    int local_index = (i - I);
                    if(flip_pixel(x + j, y + local_index))
                        V[0xF] = 1;
                } 
            }
        }
        break;
    default:
        return 0;
    }
    return 1;
}

bool CPU::flip_pixel(int x, int y) {
    SDL_LogTrace(SDL_LOG_CATEGORY_APPLICATION, "Flip: (%d, %d)", x, y);
    if (x < 0) x += width;
    else if (x >= width) x -= width;

    if (y < 0) y += height;
    else if (y >= height) y -= height;

    int i = (y * width) + x;
    frame_buffer[i] = !frame_buffer[i];
    return !frame_buffer[i];
}