#include "myTerm.h"
#include <unistd.h>

int mt_delline(void) {
    const char *clear_line = "\033[K";
    if (write(STDOUT_FILENO, clear_line, 3) == -1) {
        return -1;
    }
    return 0;
}