#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <hardwares.h>
#include <chip8.h>

int main() {

    // const uint16_t memory_length = 0x1000;
    // uint8_t memory_array[memory_length];
    // initialize_memory(memory_length, memory_array);

    // memory_array[0x00F] = 0x40;
    // memory_array[0x01F] = 0xA6;
    // memory_array[0x01D] = 0xA7;
    // print_memory(16, memory_length, memory_array);
    // printf("0x%02X\n", read(0x00F, memory_array, memory_length));

    // Memory memory;
    // uint8_t memory_arr[0x1000];
    // initialize_memory(0x1000);

    printf("%d\n", add_two(2, 5));
    // for (int i = 0; i < 5; i++)
    //     printf("%d ", memory->arr[i]);
    // memory->arr[5] = 5;

    
    // printf("%d\n", memory->arr[5]);

    // free(memory->arr);
    // free(memory);
    return 0;
}