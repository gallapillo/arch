#include "sc.h"
#include <stdio.h>

void sc_printCell(int address, enum colors fg, enum colors bg) {
    if (address < 0 || address >= MEMORY_SIZE) {
        return;
    }
    
    // Вычисляем позицию на экране
    int row = MEMORY_START_ROW + address / MEMORY_COLS;
    int col = MEMORY_START_COL + (address % MEMORY_COLS) * 6;
    
    mt_gotoXY(row, col);
    mt_setfgcolor(fg);
    mt_setbgcolor(bg);
    
    int value = memory[address];
    int sign, command, operand;
    sc_commandDecode(value, &sign, &command, &operand);
    
    printf("%c%02X%02X", sign ? '-' : '+', command, operand);
    
    mt_setdefaultcolor();
}