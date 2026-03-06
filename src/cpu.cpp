#include <iostream>
#include <random>
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
      DT(0),
      ST(0),
      SP(0),
      sub_stack(),
      V(),
      width(64),
      height(32),
      frame_buffer(),
      memory(m),
      mt(rd()),
      dist(0, 0x100)
{
    scancode_to_key.fill(-1);
    scancode_to_key[SDL_SCANCODE_1] = 0x1;
    scancode_to_key[SDL_SCANCODE_2] = 0x2;
    scancode_to_key[SDL_SCANCODE_3] = 0x3;
    scancode_to_key[SDL_SCANCODE_Q] = 0x4;
    scancode_to_key[SDL_SCANCODE_W] = 0x5;
    scancode_to_key[SDL_SCANCODE_E] = 0x6;
    scancode_to_key[SDL_SCANCODE_A] = 0x7;
    scancode_to_key[SDL_SCANCODE_S] = 0x8;
    scancode_to_key[SDL_SCANCODE_D] = 0x9;
    scancode_to_key[SDL_SCANCODE_Z] = 0xA;
    scancode_to_key[SDL_SCANCODE_X] = 0x0;
    scancode_to_key[SDL_SCANCODE_C] = 0xB;
    scancode_to_key[SDL_SCANCODE_4] = 0xC;
    scancode_to_key[SDL_SCANCODE_R] = 0xD;
    scancode_to_key[SDL_SCANCODE_F] = 0xE;
    scancode_to_key[SDL_SCANCODE_V] = 0xF;
    key_to_scancode[0x0] = SDL_SCANCODE_X;
    key_to_scancode[0x1] = SDL_SCANCODE_1;
    key_to_scancode[0x2] = SDL_SCANCODE_2;
    key_to_scancode[0x3] = SDL_SCANCODE_3;
    key_to_scancode[0x4] = SDL_SCANCODE_Q;
    key_to_scancode[0x5] = SDL_SCANCODE_W;
    key_to_scancode[0x6] = SDL_SCANCODE_E;
    key_to_scancode[0x7] = SDL_SCANCODE_A;
    key_to_scancode[0x8] = SDL_SCANCODE_S;
    key_to_scancode[0x9] = SDL_SCANCODE_D;
    key_to_scancode[0xA] = SDL_SCANCODE_Z;
    key_to_scancode[0xB] = SDL_SCANCODE_C;
    key_to_scancode[0xC] = SDL_SCANCODE_4;
    key_to_scancode[0xD] = SDL_SCANCODE_R;
    key_to_scancode[0xE] = SDL_SCANCODE_F;
    key_to_scancode[0xF] = SDL_SCANCODE_V;
}

int reverse(int b) {
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
}

void CPU::op_Dxyn(int x, int y, int n) {
    bool collision = false;
    for (int i = I; i < I + n; i++) {
        int byte = memory[i];
        byte = reverse(byte);
            
        for (int j = 0; j < 8; j++) {
            bool bit = (byte >> j) & 1;
            SDL_LogTrace(SDL_LOG_CATEGORY_APPLICATION, "bit: : %d i: %d j: %d", bit, i, j);
            if (bit) {
                int local_index = (i - I);
                int coord_x = V[x];
                int coord_y = V[y];
                if(flip_pixel(coord_x + j, coord_y + local_index))
                    collision = true;
            } 
        }
    }
    V[0xF] = collision;
}

bool CPU::execute(int opcode) {
    int id = opcode >> 12;
    int x = (opcode & 0xF00) >> 8;
    int y = (opcode & 0xF0) >> 4;
    int n = opcode & 0xF;
    int kk = opcode & 0xFF;
    int nnn = opcode & 0xFFF;

    switch (id) {
        case 0x0: {
            if (nnn == 0x0E0) {
                for (int i = 0; i < frame_buffer.size(); i++) {
                    frame_buffer[i] = false;
                }
            }
            else if (nnn == 0x0EE) {
                if (SP - 1 == 0x10) throw std::out_of_range("CPU::execute(int opcode) stack underflow");
                PC = sub_stack[--SP];
                sub_stack[SP] = 0;
            }
            else if (nnn == 0x0FD) return 0; // EXIT opcode from Super Chip-48
            break;
        }
        case 0x1: {
            PC = nnn - 2;
            break;
        }
        case 0x2: {
            if (SP + 1 == 0x10) throw std::out_of_range("CPU::execute(int opcode) stack overflow");
            sub_stack[SP++] = PC;
            PC = nnn - 2;
            break;
        }
        case 0x3: {
            if (V[x] == kk) PC += 2;
            break;
        }
        case 0x4: {
            if (V[x] != kk) PC += 2;
            break;
        }
        case 0x5: {
            if (V[x] == V[y]) PC += 2;
            break;
        }
        case 0x6: {
            V[x] = kk;
            break;
        }
        case 0x7: {
            V[x] += kk;
            break;
        }
        case 0x8: {
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
        }
        case 0x9: {
            if (V[x] != V[y]) PC += 2;
            break;
        }
        case 0xA: {
            I = nnn;
            break;
        }
        case 0xB: {
            PC = (nnn + V[0]) - 2;
            break;
        }
        case 0xC: {
            V[x] = get_random() & kk;
        }
        case 0xD: {
            op_Dxyn(x, y, n);
            break;
        }
        case 0xE: {
            int numkeys = 0;
            const bool* keyboard_state = SDL_GetKeyboardState(&numkeys);
            bool vx_pressed = false;
            if (V[x] < numkeys)
                vx_pressed = keyboard_state[key_to_scancode[V[x]]];
            if (kk == 0x9E) {
                if (vx_pressed)
                    PC += 2;
            } else if  (kk == 0xA1) {
                if (!vx_pressed)
                    PC += 2;
            } else {
                return 0;
            }
            break;
        }
        case 0xF: {
            switch (kk) {
                case 0x07: {
                    V[x] = DT;
                    break;
                }
                case 0x0A: {
                    SDL_Event event;
                    
                    bool is_valid_key = false;
                    int scancode_pressed = -1;
                    while (event.type != SDL_EVENT_KEY_DOWN && !is_valid_key) {
                        while (SDL_PollEvent(&event)) {
                            scancode_pressed = event.key.scancode;
                            for (int i = 0; i < key_to_scancode.size(); i++)
                                if (key_to_scancode[i] == scancode_pressed)
                                    is_valid_key = true;
                                else if (scancode_pressed == SDL_SCANCODE_ESCAPE)
                                    return 0;
                        }
                    }
                    V[x] = scancode_to_key[scancode_pressed];
                    break;
                }
                case 0x15: {
                    DT = V[x];
                    break;
                }
                case 0x18: {
                    ST = V[x];
                    break;
                }
                case 0x1E: {
                    I = (I + V[x]) & 0xFFF;
                    break;
                }
                case 0x29: {
                    I = V[x] * 5;
                    break;
                }
                case 0x33: {
                    break;
                }
                case 0x55: {
                    break;
                }
                case 0x65: {
                    break;
                }
            }
            break;
        }
        default: {
            return 0;
        }
    }
    return 1;
}

bool CPU::flip_pixel(int x, int y) {
    SDL_LogTrace(SDL_LOG_CATEGORY_APPLICATION, "Flip: (%d, %d)", x, y);

    // modulo width/height to wrap out-of-bounds coords
    x %= width;
    y %= height;

    int i = (y * width) + x;
    frame_buffer[i] = !frame_buffer[i];
    return !frame_buffer[i];
}

int CPU::get_random() { return dist(mt); }
