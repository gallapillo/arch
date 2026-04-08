#include "sc.h"
#include <stdlib.h>

int sc_commandEncode(int sign, int command, int operand, int *value) {
    if (value == NULL) {
        return -1;
    }
    
    if (sign != 0 && sign != 1) {
        return -1;
    }
    
    if (command < 0 || command > MAX_COMMAND_VALUE) {
        return -1;
    }
    
    if (operand < 0 || operand > MAX_OPERAND_VALUE) {
        return -1;
    }
    
    // Формат: бит 15 - знак, биты 14-9 - команда, биты 8-0 - операнд
    *value = (sign << 15) | (command << 9) | operand;
    return 0;
}