#ifndef MEMORY_H
#define MEMORY_H

#include <array>

using namespace std;

class Memory {
public:
    Memory();
    int& operator[](int address);
    int read_16(int address);
    void print();
    void print(int start, int end);
private:
    array<int, 0x1000> elem;
};

#endif
