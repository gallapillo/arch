#include "myBigChars.h"
#include <unistd.h>

int bc_printA(char *str) {
    if (str == NULL) {
        return -1;
    }
    
    // Вход в режим альтернативной кодировки
    if (write(STDOUT_FILENO, "\033(0", 3) == -1) {
        return -1;
    }
    
    // Вывод строки
    while (*str) {
        if (write(STDOUT_FILENO, str, 1) == -1) {
            return -1;
        }
        str++;
    }
    
    // Выход из режима альтернативной кодировки
    if (write(STDOUT_FILENO, "\033(B", 3) == -1) {
        return -1;
    }
    
    return 0;
}