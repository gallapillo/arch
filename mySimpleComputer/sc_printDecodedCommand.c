#include "sc.h"
#include <stdio.h>

void sc_printDecodedCommand(int value) {
    mt_gotoXY(DECODED_ROW, DECODED_COL);
    mt_setfgcolor(COLOR_MAGENTA);
    
    int sign, command, operand;
    sc_commandDecode(value, &sign, &command, &operand);
    
    printf("DEC: %c%02X%02X", sign ? '-' : '+', command, operand);
    printf(" | DEC10: %d | HEX: 0x%04X | BIN: ", value, value & 0xFFFF);
    
    for (int i = 15; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
        if (i % 4 == 0 && i != 0) printf(" ");
    }
    
    mt_setdefaultcolor();
}