#ifndef CPU_H
#define CPU_H

#include <cstdint>

class CPU {
public:
    CPU();
    int execute(int opcode);

    int PC;       // Program Counter
    int I;        // Special 16-bit register
    int delay;     // Delay register
    int sound;     // Sound register
    int SP;        // Stack Pointer
    int* stack;   // Subroutine stack
    int* V;        // 16 8-bit General purpose registers
};

#endif
