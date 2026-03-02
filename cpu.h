#ifndef CPU_H
#define CPU_H

#include <cstdint>

class CPU {
public:
    CPU();
    int execute(int opcode);

    uint_fast16_t PC;       // Program Counter
    uint_fast16_t I;        // Special 16-bit register
    uint_fast8_t delay;     // Delay register
    uint_fast8_t sound;     // Sound register
    uint_fast8_t SP;        // Stack Pointer
    uint_fast16_t* stack;   // Subroutine stack
    uint_fast8_t* V;        // 16 8-bit General purpose registers
};

#endif
