#include "sc.h"
#include <stdio.h>

void sc_printFlags(void) {
    mt_gotoXY(FLAGS_ROW, FLAGS_COL);
    mt_setfgcolor(COLOR_YELLOW);
    printf("FLAGS: ");
    
    printf("%c", (flags & FLAG_OVERFLOW) ? 'P' : '_');
    printf("%c", (flags & FLAG_DIVISION) ? 'O' : '_');
    printf("%c", (flags & FLAG_MEMORY)   ? 'M' : '_');
    printf("%c", (flags & FLAG_IGNORE)   ? 'T' : '_');
    printf("%c", (flags & FLAG_HALT)     ? 'E' : '_');
    
    mt_setdefaultcolor();
}