#include "myBigChars.h"
#include <stdio.h>
#include <unistd.h>

int bc_printbigchar(int *big, int x, int y, enum colors fg, enum colors bg) {
    if (big == NULL) {
        return -1;
    }
    
    mt_setfgcolor(fg);
    mt_setbgcolor(bg);
    
    // Вход в режим псевдографики для символа закрашивания
    write(STDOUT_FILENO, "\033(0", 3);
    
    for (int row = 0; row < 8; row++) {
        mt_gotoXY(x + row, y);
        
        int *target = (row < 4) ? &big[0] : &big[1];
        int byte = (row % 4) * 8;
        
        for (int col = 0; col < 8; col++) {
            int bit = (target[0] >> (byte + col)) & 1;
            if (bit) {
                write(STDOUT_FILENO, "h", 1);  // ACS_CKBOARD
            } else {
                write(STDOUT_FILENO, " ", 1);
            }
        }
    }
    
    // Выход из режима псевдографики
    write(STDOUT_FILENO, "\033(B", 3);
    mt_setdefaultcolor();
    
    return 0;
}