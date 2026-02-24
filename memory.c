#include <stdio.h>
#include <stdint.h>

void initialize_memory(uint16_t memory_length, uint8_t memory_array[]) {
    for (int i = 0; i < memory_length; i++) {
        memory_array[i] = 0x00;
    }
}

void print_memory(int row_length, int memory_length, int memory_arr[]) {
    printf("%03X  ", 0);
    for (int i = 0; i < memory_length; i++) {
        int row_arr[row_length];
        printf("%02X ", memory_arr[i]);
        if ((i + 1) % row_length == 0 && i < memory_length - 1)
            printf("\n%03X  ", i + 1);
        else if ((i + 1) % 4 == 0)
            printf(" ");
    }
    printf("\n");
}

uint8_t mem_read(u_int16_t address, uint8_t memory_arr[]) {
    if (address < (sizeof(*memory_arr) / sizeof(memory_arr[0])) && address >= 0)
        return memory_arr[address];
    return -1;
}

int main() {
    const uint16_t memory_length = 0x1000;
    uint8_t memory_array[memory_length];
    initialize_memory(memory_length, memory_array);

    memory_array[0x00F] = 0x02;
    memory_array[0x01F] = 0xA6;
    memory_array[0x01D] = 0xA7;
    // print_memory(16, memory_length, memory_array);
    printf("0x%02X\n", mem_read(0x00F, memory_array));

    // printf("%d\n", memory_array[0xFFE]);
    return 0;
}
