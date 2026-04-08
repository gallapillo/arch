#include "myTerm.h"
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

int mt_getscreensize(int *rows, int *cols) {
    struct winsize ws;
    
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1) {
        return -1;
    }
    
    if (rows != NULL) {
        *rows = ws.ws_row;
    }
    if (cols != NULL) {
        *cols = ws.ws_col;
    }
    
    return 0;
}