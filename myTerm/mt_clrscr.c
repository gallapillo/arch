#include "myTerm.h"
#include <unistd.h>

int mt_clrscr(void) {
    const char *clear = "\033[H\033[J";
    if (write(STDOUT_FILENO, clear, 6) == -1) {
        return -1;
    }
    return 0;
}