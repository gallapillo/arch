#include "sc.h"

int sc_commandValidate(int command) {
    // Допустимые команды: 0x00-0x3F
    if (command < 0 || command > MAX_COMMAND_VALUE) {
        return -1;
    }
    return 0;
}