#include "myTerm.h"
#include <stdio.h>
#include <unistd.h>

int mt_setbgcolor(enum colors color) {
    if (color < COLOR_BLACK || color > COLOR_DEFAULT) {
        return -1;
    }
    
    char buffer[16];
    int len;
    
    if (color == COLOR_DEFAULT) {
        len = snprintf(buffer, sizeof(buffer), "\033[49m");
    } else {
        len = snprintf(buffer, sizeof(buffer), "\033[%dm", color + 40);
    }
    
    if (write(STDOUT_FILENO, buffer, len) == -1) {
        return -1;
    }
    return 0;
}