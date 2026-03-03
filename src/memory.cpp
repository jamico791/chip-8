#include <iostream>
#include <iomanip>
#include <stdexcept>

#include "memory.h"

using namespace std;

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
