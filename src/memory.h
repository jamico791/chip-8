#ifndef MEMORY_H
#define MEMORY_H

#include <array>
#include <vector>

using namespace std;

class Memory {
public:
    Memory();
    int& operator[](int address);
    int read_16(int address);
    void print();
    void print(int start, int end);
    vector<int> read_n(int n, int start);
    void write_n(vector<int> nums, int start);
private:
    array<int, 0x1000> elem;
};

#endif
