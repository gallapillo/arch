#include "myReadkey.h"
#include <unistd.h>
#include <stdlib.h>

int rk_mytermsave(void) {
    if (tcgetattr(STDIN_FILENO, &saved_termios) == -1) {
        return -1;
    }
    return 0;
}