#include "myBigChars.h"
#include <stdio.h>
#include <unistd.h>

int bc_printbigchar(int *big, int x, int y, enum colors fg, enum colors bg) {
    if (big == NULL) {
        return -1;
    }
    
    mt_setfgcolor(fg);
    mt_setbgcolor(bg);
    
    for (int row = 0; row < 8; row++) {
        mt_gotoXY(x + row, y);
        
        int *target = (row < 4) ? &big[0] : &big[1];
        int byte = (row % 4) * 8;
        
        for (int col = 0; col < 8; col++) {
            int bit = (target[0] >> (byte + col)) & 1;
            if (bit) {
                printf("█");  // Закрашенный блок (UTF-8)
            } else {
                printf(" ");
            }
        }
    }
    
    mt_setdefaultcolor();
    return 0;
}