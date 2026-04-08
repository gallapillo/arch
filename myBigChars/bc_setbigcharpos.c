#include "myBigChars.h"
#include <stdlib.h>

int bc_setbigcharpos(int *big, int x, int y, int value) {
    if (big == NULL || x < 0 || x >= 8 || y < 0 || y >= 8) {
        return -1;
    }
    
    int *target = (x < 4) ? &big[0] : &big[1];
    int bit_pos = (x % 4) * 8 + y;
    
    if (value) {
        *target |= (1 << bit_pos);
    } else {
        *target &= ~(1 << bit_pos);
    }
    
    return 0;
}