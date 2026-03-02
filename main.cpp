#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include <cstdint>

#include "chip8.h"

using namespace std;


int main() {
    Chip8 chip8 = Chip8();

    chip8.read_program("test.ch8");
    chip8.memory.print(0x200, 0x2FF);

    chip8.run();

    return 0;
}
