#ifndef CHIP8_H
#define CHIP8_H

#include "memory.h"
#include "cpu.h"

class Chip8 {
public:
    Chip8();
    void print_cpu();
    int cpu_step();
    void run();

    Memory memory;
    CPU cpu;
};

#endif
