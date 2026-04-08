#include "myBigChars.h"
#include <stdlib.h>

int bc_getbigcharpos(int *big, int x, int y, int *value) {
    if (big == NULL || value == NULL || x < 0 || x >= 8 || y < 0 || y >= 8) {
        return -1;
    }
    
    int *target = (x < 4) ? &big[0] : &big[1];
    int bit_pos = (x % 4) * 8 + y;
    
    *value = (*target >> bit_pos) & 1;
    
    return 0;
}