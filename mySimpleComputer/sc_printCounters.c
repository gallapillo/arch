#include "sc.h"
#include <stdio.h>

void sc_printCounters(void) {
    mt_gotoXY(ICOUNTER_ROW, ICOUNTER_COL);
    mt_setfgcolor(COLOR_GREEN);
    printf("IC: %d (0x%04X)", icounter, icounter & 0xFFFF);
    mt_setdefaultcolor();
}