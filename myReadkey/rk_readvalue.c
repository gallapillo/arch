#include "myReadkey.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/select.h>
#include <ctype.h>

int rk_readvalue(int *value, int timeout) {
    if (value == NULL) {
        return -1;
    }
    
    char buf[16];
    int pos = 0;
    fd_set readfds;
    struct timeval tv;
    
    *value = 0;
    
    while (1) {
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        tv.tv_sec = timeout / 1000;
        tv.tv_usec = (timeout % 1000) * 1000;
        
        int ready = select(STDIN_FILENO + 1, &readfds, NULL, NULL, 
                           timeout >= 0 ? &tv : NULL);
        
        if (ready == -1) {
            return -1;
        }
        
        if (ready == 0) {
            // Таймаут
            if (pos == 0) {
                return -1;
            }
            buf[pos] = '\0';
            sscanf(buf, "%x", value);
            return 0;
        }
        
        char ch;
        if (read(STDIN_FILENO, &ch, 1) != 1) {
            return -1;
        }
        
        if (ch == '\n') {
            buf[pos] = '\0';
            if (pos == 0) {
                return -1;
            }
            sscanf(buf, "%x", value);
            return 0;
        }
        
        if (ch == '\033') {  // ESC
            return -1;
        }
        
        // Допустимые символы: 0-9, A-F, a-f
        if (isxdigit((unsigned char)ch) && pos < 15) {
            buf[pos++] = ch;
            write(STDOUT_FILENO, &ch, 1);
        }
    }
}