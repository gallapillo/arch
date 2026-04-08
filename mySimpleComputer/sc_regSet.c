#include "sc.h"

int sc_regSet(int reg, int value) {
    int mask;
    switch (reg) {
        case REG_FLAG_OVERFLOW: mask = FLAG_OVERFLOW; break;
        case REG_FLAG_DIVISION: mask = FLAG_DIVISION; break;
        case REG_FLAG_MEMORY:   mask = FLAG_MEMORY;   break;
        case REG_FLAG_IGNORE:   mask = FLAG_IGNORE;   break;
        case REG_FLAG_HALT:     mask = FLAG_HALT;     break;
        default: return -1;
    }
    
    if (value) {
        flags |= mask;
    } else {
        flags &= ~mask;
    }
    return 0;
}