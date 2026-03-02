#ifndef CHIP8_H
#define CHIP8_H

#include <string>

#include "memory.h"
#include "cpu.h"

using namespace std;

class Chip8 {
public:
    Chip8();
    ~Chip8();
    void read_program(string filename);
    void print_cpu();
    int cpu_step();
    void run();

    Memory memory;
    CPU cpu;
};

#endif
