#include <cstdint>
#include <stdexcept>

#include "cpu.h"

CPU::CPU() {
    PC = 0x200;
    I = 0;
    delay = 0;
    sound = 0;
    SP = 0;
    stack = new int[0x10];
    V = new int[0x10];

    for (int i = 0; i < 0x10; i++) {
        V[i] = 0;
        stack[i] = 0;
    }
}

int CPU::execute(int opcode) {
    int id = opcode >> 12;
    int x = (opcode & 0xF00) >> 8;
    int y = (opcode & 0xF0) >> 4;
    int n = opcode & 0xF;
    int kk = opcode & 0xFF;
    int nnn = opcode & 0xFFF;

    switch (id) {
    case 0x0:
        if (nnn == 0x0E0) return 0;       // CLS  TODO: Implement clear display logic
        else if (nnn == 0x0EE) {
            if (SP - 1 == 0x10) throw std::out_of_range("CPU::execute(int opcode) stack underflow");
            PC = stack[--SP];
            stack[SP] = 0;
        }
        else if (nnn == 0x0FD) return -1; // EXIT opcode from Super Chip-48
        break;
    case 0x1:
        PC = nnn - 2;
        break;
    case 0x2:
        if (SP + 1 == 0x10) throw std::out_of_range("CPU::execute(int opcode) stack overflow");
        stack[SP++] = PC;
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
    default:
        return -1;
    }
    return 0;
}
