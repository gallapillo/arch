#include "myBigChars.h"
#include <stdlib.h>

int bc_strlen(char *str) {
    if (str == NULL) {
        return 0;
    }
    
    int count = 0;
    unsigned char *s = (unsigned char *)str;
    
    while (*s) {
        // Проверка корректности UTF-8
        if ((*s & 0x80) == 0) {
            // 1 байт (ASCII)
            s += 1;
        } else if ((*s & 0xE0) == 0xC0) {
            // 2 байта
            if ((s[1] & 0xC0) != 0x80) return 0;
            s += 2;
        } else if ((*s & 0xF0) == 0xE0) {
            // 3 байта
            if ((s[1] & 0xC0) != 0x80 || (s[2] & 0xC0) != 0x80) return 0;
            s += 3;
        } else if ((*s & 0xF8) == 0xF0) {
            // 4 байта
            if ((s[1] & 0xC0) != 0x80 || (s[2] & 0xC0) != 0x80 || 
                (s[3] & 0xC0) != 0x80) return 0;
            s += 4;
        } else {
            return 0; // Некорректный UTF-8
        }
        count++;
    }
    
    return count;
}