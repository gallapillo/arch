#include "sc.h"
#include <stdio.h>

void sc_printAccumulator(void) {
    mt_gotoXY(ACCUM_ROW, ACCUM_COL);
    mt_setfgcolor(COLOR_CYAN);
    printf("ACC: %d (0x%04X)", accumulator, accumulator & 0xFFFF);
    mt_setdefaultcolor();
}