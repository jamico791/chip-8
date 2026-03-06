#ifndef CHIP8_H
#define CHIP8_H

#include <string>

#include "memory.h"
#include "cpu.h"
#include "display.h"

using namespace std;

class Chip8 {
public:
    Chip8();
    void read_program(string filename);
    int cpu_step(int key_pressed);
    void init();
    void loop(bool& running);
    void shutdown();
    void step();
    void print_cpu();

private:
    int width;
    int height;
    int scale;
    bool should_step;

    Memory memory;
    CPU cpu;
    Display display;
};

#endif
