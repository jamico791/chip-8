#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include <cstdint>

using namespace std;

class Memory {
public:
    Memory(int size);
    Memory();
    int& operator[](int address);
    int read_16(int address);
    void print();
    void print(int start, int end);
private:
    int* elem;
    int sz;
};

Memory::Memory(int size) {
    elem = new int[size];
    sz = size;

    for (int i = 0; i != size; ++i) {
        elem[i] = 0;
    }
}

Memory::Memory() {
    sz = 0x1000;
    elem = new int[sz];

    for (int i = 0; i != sz; ++i) {
        elem[i] = 0;
    }
}

int& Memory::operator[](int address) {
    if (address < 0 || address >= sz) throw out_of_range("Memory::read");
    return elem[address];
}

int Memory::read_16(int address) {
    if (address < 0 || address >= sz) throw out_of_range("Memory::read");
    int high = elem[address] << 8;
    int low = elem[address + 1];
    return high + low;
}

void Memory::print(int start, int end) {
    if (end < start) throw out_of_range("Memory::print(int start, int end)");
    int new_start = start - (start % 16);

    for (int i = new_start; i <= end; ++i) {
        if (i != new_start) {
            if (i % 4 == 0) cout << ' ';
            if (i % 16 == 0) cout << '\n';
        } 
        if (i % 16 == 0) {
            cout << setfill('0') << setw(3) << uppercase << hex << i << "  ";
        }
        if (i < start) cout << "   ";
        else cout << setfill('0') << setw(2) << uppercase << hex << elem[i] << " ";
    }
    cout << endl;
}

void Memory::print() {
    print(0, sz - 1);
}

class CPU {
public:
    CPU();
    void print();
    int execute(int opcode);

    uint_fast16_t PC;       // Program Counter
    uint_fast16_t I;        // Special 16-bit register
    uint_fast8_t delay;     // Delay register
    uint_fast8_t sound;     // Sound register
    uint_fast8_t SP;        // Stack Pointer
    uint_fast16_t* stack;   // Subroutine stack
    uint_fast8_t* V;        // 16 8-bit General purpose registers
};

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

void CPU::print() {
    cout << setfill('0') << uppercase << hex;
    cout << "PC: " << PC << '\n';
    cout << "I: " << setw(3) << I << '\n';
    cout << "delay: " << setw(2) << static_cast<unsigned>(delay) << '\n';
    cout << "sound: " << setw(2) << static_cast<unsigned>(sound) << '\n';
    cout << "SP: " << setw(2) << static_cast<unsigned>(SP) << '\n';
    cout << "\nStack:" << '\n';
    for (int i = 0; i < 0x10; i++) {
        if (i == 8) cout << '\n';
        cout << setw(3) << stack[i] << " ";
    }
    cout << '\n' << "\nGP Registers:" << '\n';
    for (int i = 0; i < 0x10; i++) {
        if (i != 0 && i % 2 == 0) cout << '\n';
        cout << "V" << i << ": " << setw(2) << static_cast<unsigned>(V[i]) << "  ";
    }
    cout << endl;
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
        if (kk == 0xFD) return -1; // EXIT opcode from Super Chip-48
        break;
    case 0x1:
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

class Chip8 {
public:
    Chip8();
    void print_cpu();
    int cpu_step();
    void run();

    Memory memory;
    CPU cpu;
};

Chip8::Chip8() {
    memory = Memory(0x1000);
    cpu = CPU();
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
    while (rc != -1) {
        print_cpu();
        rc = cpu_step();
        cycle++;
    }
}

void Chip8::print_cpu() {
    cout << "CPU State\n-----------\n";
    cout << setfill('0') << uppercase << hex;
    cout << "PC: " << cpu.PC << "  Opcode: " << memory.read_16(cpu.PC) << '\n';
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