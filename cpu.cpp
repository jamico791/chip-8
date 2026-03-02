#include <cstdint>
#include <stdexcept>

#include "cpu.h"

CPU::CPU() {
    PC = 0x200;
    I = 0;
    delay = 0;
    sound = 0;
    SP = 0;
    stack = new uint_fast16_t[0x10];
    V = new uint_fast8_t[0x10];

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
    case 0x6:
        V[x] = kk;
        break;
    case 0xA:
        I = nnn;
        break;
    default:
        return -1;
    }
    return 0;
}
