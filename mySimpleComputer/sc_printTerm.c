#include "sc.h"
#include <stdio.h>
#include <string.h>

// Статический буфер для хранения последних строк
static char term_buffer[TERM_LINES + 1][80];
static int term_index = 0;

void sc_printTerm(int address, int isInput) {
    int value;
    sc_memoryGet(address, &value);
    
    // Сдвигаем буфер вверх
    if (term_index >= TERM_LINES) {
        for (int i = 1; i < TERM_LINES; i++) {
            strcpy(term_buffer[i - 1], term_buffer[i]);
        }
        term_index = TERM_LINES - 1;
    }
    
    // Формируем новую строку
    if (isInput) {
        snprintf(term_buffer[term_index], sizeof(term_buffer[0]), 
                 "[0x%04X] > ", address);
    } else {
        snprintf(term_buffer[term_index], sizeof(term_buffer[0]), 
                 "[0x%04X] = 0x%04X (%d)", address, value & 0xFFFF, value);
    }
    
    // Выводим все строки
    mt_setfgcolor(COLOR_WHITE);
    mt_setbgcolor(COLOR_BLACK);
    
    for (int i = 0; i <= term_index && i < TERM_LINES; i++) {
        mt_gotoXY(TERM_START_ROW + i, TERM_START_COL);
        mt_delline();
        printf("%s", term_buffer[i]);
    }
    
    mt_setdefaultcolor();
    term_index++;
}