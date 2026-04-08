#include "sc.h"
#include <stdio.h>

void sc_printCommand(void) {
    mt_gotoXY(COMMAND_ROW, COMMAND_COL);
    mt_setfgcolor(COLOR_GREEN);
    
    int value;
    sc_memoryGet(icounter, &value);
    
    int sign, command, operand;
    sc_commandDecode(value, &sign, &command, &operand);
    
    if (sc_commandValidate(command) == -1) {
        printf("! %c%02X%02X", sign ? '-' : '+', command, operand);
    } else {
        printf("CMD: %c%02X%02X", sign ? '-' : '+', command, operand);
    }
    
    mt_setdefaultcolor();
}