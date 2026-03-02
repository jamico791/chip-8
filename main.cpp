#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include <cstdint>

#include "chip8.h"

using namespace std;


int main() {
    Chip8 chip8 = Chip8();

    // ifstream program;
    // program.open("test_opcode.ch8");
    // program.read();

    chip8.memory[0x200] = 0xA2;
    chip8.memory[0x201] = 0x16;
    chip8.memory[0x202] = 0x12;
    chip8.memory[0x203] = 0x16;
    chip8.memory[0x216] = 0x63;
    chip8.memory[0x217] = 0x05;
    chip8.memory[0x218] = 0x00;
    chip8.memory[0x219] = 0xFD;


    chip8.run();

    // memory.print();
    return 0;
}
