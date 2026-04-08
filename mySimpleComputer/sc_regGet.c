#include "sc.h"
#include <stdlib.h>

int sc_regGet(int reg, int *value) {
    if (value == NULL) {
        return -1;
    }
    
    int mask;
    switch (reg) {
        case REG_FLAG_OVERFLOW: mask = FLAG_OVERFLOW; break;
        case REG_FLAG_DIVISION: mask = FLAG_DIVISION; break;
        case REG_FLAG_MEMORY:   mask = FLAG_MEMORY;   break;
        case REG_FLAG_IGNORE:   mask = FLAG_IGNORE;   break;
        case REG_FLAG_HALT:     mask = FLAG_HALT;     break;
        default: return -1;
    }
    
    *value = (flags & mask) ? 1 : 0;
    return 0;
}