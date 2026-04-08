#include "myTerm.h"
#include <unistd.h>

int mt_setdefaultcolor(void) {
    const char *default_color = "\033[0m";
    if (write(STDOUT_FILENO, default_color, 4) == -1) {
        return -1;
    }
    return 0;
}