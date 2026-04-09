#include "sc.h"
#include <stdio.h>

void sc_printCell(int address, enum colors fg, enum colors bg) {
    if (address < 0 || address >= MEMORY_SIZE) {
        return;
    }
    
    // Вычисляем позицию на экране
    int row = MEMORY_START_ROW + (address / MEMORY_COLS);
    int col = MEMORY_START_COL + ((address % MEMORY_COLS) * 6);
    
    // Перемещаем курсор в нужную позицию
    mt_gotoXY(row, col);
    
    // Устанавливаем цвета
    mt_setfgcolor(fg);
    mt_setbgcolor(bg);
    
    // Получаем значение ячейки
    int value;
    sc_memoryGet(address, &value);
    
    int sign, command, operand;
    sc_commandDecode(value, &sign, &command, &operand);
    
    // Выводим ячейку в формате +XXXXX (5 символов)
    printf("%c%04X", sign ? '-' : '+', value & 0xFFFF);
    
    // Сбрасываем цвета
    mt_setdefaultcolor();
    
    fflush(stdout);
}