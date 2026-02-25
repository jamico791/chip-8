#ifndef HARDWARE_H
#define HARDWARE_H

#include <stdint.h>

typedef struct {
    uint8_t arr[0x1000];
} Memory;

typedef struct {
    Memory *memory;
} Chip8;

#endif