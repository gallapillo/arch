#include "sc.h"

int sc_memorySet(int address, int value) {
    if (address < 0 || address >= MEMORY_SIZE) {
        return -1;
    }
    memory[address] = value;
    return 0;
}