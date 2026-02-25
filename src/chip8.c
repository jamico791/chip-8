#include <chip8.h>
#include <hardwares.h>
#include <stdint.h>
#include <stdio.h>

// Returns a pointer to a memory structure with the specified length
void initialize_memory(Memory *memory, uint16_t length) {
    for (int i = 0; i < length; i++) {
        memory->arr[i] = 0;
    }
//     struct Memory *memory;
//     memory = malloc(sizeof(struct Memory));

//     if (memory == NULL) {
//         printf("Allocation of memory Failed\n");
//         exit(0);
//     }
//     memory->length = length;
//     printf("%d\n", memory->length);
//     printf("%ld\n", sizeof(memory->length));
//     printf("%ld\n", sizeof(*memory));
//     printf("%ld\n", sizeof(*memory));
//     memory->arr = (uint8_t *)(length, sizeof(uint8_t));

//     if (memory->arr == NULL) {
//         printf("Allocation Failed");
//         exit(0);
//     }

//     return memory;
}

int add_two(int a, int b) {
    return a + b;
}

void print_memory(int row_length, uint16_t memory_length, uint8_t memory_arr[]) {
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

uint8_t read(uint16_t address, uint8_t memory_arr[], uint16_t memory_length) {
    if (address >= 0 && address < memory_length)
        return memory_arr[address];
    return -1;
}
