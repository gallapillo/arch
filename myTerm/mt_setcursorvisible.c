#include "myTerm.h"
#include <unistd.h>

int mt_setcursorvisible(int value) {
    const char *hide = "\033[?25l";
    const char *show = "\033[?25h";
    
    if (value) {
        if (write(STDOUT_FILENO, show, 6) == -1) {
            return -1;
        }
    } else {
        if (write(STDOUT_FILENO, hide, 6) == -1) {
            return -1;
        }
    }
    return 0;
}