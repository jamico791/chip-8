#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>

#include "chip8.h"

using namespace std;

Chip8::Chip8() {
    memory = Memory(0x1000);
    cpu = CPU();
}

Chip8::~Chip8() {
    delete[] memory.elem;
    delete[] cpu.stack;
    delete[] cpu.V;
}

void Chip8::read_program(string filename) {
    ifstream file(filename, ios::binary);
    if (file) {
        int i = 0x200;
        uint8_t data;
        while (file.read(reinterpret_cast<char*>(&data), sizeof(data))) {
            memory[i] = data;
            i++;
        }
        file.close();
    } else {
        cerr << "Error opening file." << endl;
    }
}

int Chip8::cpu_step() {
    int opcode = memory.read_16(cpu.PC);
    int rc = cpu.execute(opcode);
    cpu.PC += 2;
    return rc;
}

void Chip8::run() {
    int rc = 0;
    int cycle = 0;
    print_cpu();
    while (rc != -1) {
        rc = cpu_step();
        print_cpu();
        cycle++;
    }
}

void Chip8::print_cpu() {
    cout << "-------------------------------\nCPU State\n-------------------------------\n";
    cout << setfill('0') << uppercase << hex;
    cout << setw(3) << "PC: " << cpu.PC << "  Opcode: " << setw(4) << memory.read_16(cpu.PC) << '\n';
    cout << "I: " << setw(3) << cpu.I << '\n';
    cout << "delay: " << setw(2) << static_cast<unsigned>(cpu.delay) << '\n';
    cout << "sound: " << setw(2) << static_cast<unsigned>(cpu.sound) << '\n';
    cout << "SP: " << setw(2) << static_cast<unsigned>(cpu.SP) << '\n';
    cout << "\nStack:" << '\n';

    for (int i = 0; i < 0x10; i++) {
        if (i == 8) cout << '\n';
        cout << setw(3) << cpu.stack[i] << " ";
    }
    cout << '\n' << "\nGP Registers:" << '\n';
    for (int i = 0; i < 0x10; i++) {
        if (i != 0 && i % 2 == 0) cout << '\n';
        cout << "V" << i << ": " << setw(2) << static_cast<unsigned>(cpu.V[i]) << "  ";
    }
    cout << '\n' << endl;
}
