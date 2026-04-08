#include "myReadkey.h"
#include <unistd.h>

int rk_readkey(enum keys *key) {
    char buf[32];
    ssize_t n = read(STDIN_FILENO, buf, sizeof(buf) - 1);
    
    if (n <= 0) {
        return -1;
    }
    
    buf[n] = '\0';
    
    // ESC (один символ)
    if (n == 1 && buf[0] == '\033') {
        *key = KEY_ESC;
        return 0;
    }
    
    // ENTER
    if (n == 1 && (buf[0] == '\n' || buf[0] == '\r')) {
        *key = KEY_ENTER;
        return 0;
    }
    
    // macOS/iTerm2 последовательности для стрелок
    if (n >= 2 && buf[0] == '\033' && buf[1] == '[') {
        if (n == 3) {
            switch (buf[2]) {
                case 'A': *key = KEY_UP; return 0;
                case 'B': *key = KEY_DOWN; return 0;
                case 'C': *key = KEY_RIGHT; return 0;
                case 'D': *key = KEY_LEFT; return 0;
            }
        }
        // F5: ESC [ 1 5 ~
        if (n >= 4 && buf[2] == '1' && buf[3] == '5') {
            *key = KEY_F5;
            return 0;
        }
        // F6: ESC [ 1 7 ~
        if (n >= 4 && buf[2] == '1' && buf[3] == '7') {
            *key = KEY_F6;
            return 0;
        }
        // Альтернативные последовательности для F5/F6 в некоторых терминалах macOS
        if (n == 4 && buf[2] == '1' && buf[3] == '5') {
            *key = KEY_F5;
            return 0;
        }
        if (n == 4 && buf[2] == '1' && buf[3] == '7') {
            *key = KEY_F6;
            return 0;
        }
    }
    
    // Альтернативные последовательности для стрелок (в некоторых терминалах)
    if (n >= 3 && buf[0] == '\033' && buf[1] == 'O') {
        switch (buf[2]) {
            case 'A': *key = KEY_UP; return 0;
            case 'B': *key = KEY_DOWN; return 0;
            case 'C': *key = KEY_RIGHT; return 0;
            case 'D': *key = KEY_LEFT; return 0;
        }
    }
    
    // Буквенно-цифровые клавиши
    if (n == 1) {
        char c = buf[0];
        switch (c) {
            case 'l': case 'L': *key = KEY_L; return 0;
            case 's': case 'S': *key = KEY_S; return 0;
            case 'i': case 'I': *key = KEY_I; return 0;
            case '+': *key = KEY_PLUS; return 0;
            case '-': *key = KEY_MINUS; return 0;
            case '0': *key = KEY_0; return 0;
            case '1': *key = KEY_1; return 0;
            case '2': *key = KEY_2; return 0;
            case '3': *key = KEY_3; return 0;
            case '4': *key = KEY_4; return 0;
            case '5': *key = KEY_5; return 0;
            case '6': *key = KEY_6; return 0;
            case '7': *key = KEY_7; return 0;
            case '8': *key = KEY_8; return 0;
            case '9': *key = KEY_9; return 0;
            case 'a': case 'A': *key = KEY_A; return 0;
            case 'b': case 'B': *key = KEY_B; return 0;
            case 'c': case 'C': *key = KEY_C; return 0;
            case 'd': case 'D': *key = KEY_D; return 0;
            case 'e': case 'E': *key = KEY_E; return 0;
            case 'f': case 'F': *key = KEY_F; return 0;
        }
    }
    
    *key = KEY_NONE;
    return 0;
}