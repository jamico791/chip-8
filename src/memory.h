#ifndef MEMORY_H
#define MEMORY_H

class Memory {
public:
    Memory(int size);
    Memory();
    int& operator[](int address);
    int read_16(int address);
    void print();
    void print(int start, int end);
    int* elem;
    int sz;
};

#endif
