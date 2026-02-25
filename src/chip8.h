#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>

int add_two(int a, int b);

void print_memory(int row_length, uint16_t memory_length, uint8_t memory_arr[]);

uint8_t read(uint16_t address, uint8_t memory_arr[], uint16_t memory_length);

#endif