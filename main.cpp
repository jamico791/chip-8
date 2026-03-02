#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include <cstdint>
#include <string>
#include <filesystem>

#include "chip8.h"

using namespace std;


int main(int argc, char *argv[]) {
    string filename = argv[1];
    Chip8 chip8 = Chip8();

    chip8.read_program(filename);
    chip8.memory.print(0x200, 0x2FF);

    chip8.run();

    return 0;
}
