#include "myTerm.h"
#include <stdio.h>
#include <unistd.h>

int mt_gotoXY(int row, int col) {
    if (row < 1 || col < 1) {  // В терминале строки и столбцы начинаются с 1
        return -1;
    }
    
    char buffer[32];
    int len = snprintf(buffer, sizeof(buffer), "\033[%d;%dH", row, col);
    if (write(STDOUT_FILENO, buffer, len) == -1) {
        return -1;
    }
    fflush(stdout);
    return 0;
}