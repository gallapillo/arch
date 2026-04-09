#include "myTerm.h"
#include <unistd.h>
#include <stdio.h>

int mt_clrscr(void) {
    // Для macOS
    const char *clear = "\033[2J\033[H";
    if (write(STDOUT_FILENO, clear, 7) == -1) {
        return -1;
    }
    fflush(stdout);
    return 0;
}