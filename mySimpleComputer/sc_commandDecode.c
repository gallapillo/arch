#include "sc.h"
#include <stdlib.h>

int sc_commandDecode(int value, int *sign, int *command, int *operand) {
    if (sign == NULL || command == NULL || operand == NULL) {
        return -1;
    }
    
    *sign = (value >> 15) & 1;
    *command = (value >> 9) & MAX_COMMAND_VALUE;
    *operand = value & MAX_OPERAND_VALUE;
    
    return 0;
}