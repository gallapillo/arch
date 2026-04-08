#include "myReadkey.h"
#include <unistd.h>

int rk_mytermregime(int regime, int vtime, int vmin, int echo, int sigint) {
    struct termios new_termios;
    
    if (tcgetattr(STDIN_FILENO, &new_termios) == -1) {
        return -1;
    }
    
    if (regime == 1) {  // Канонический режим
        new_termios.c_lflag |= ICANON;
        new_termios.c_cc[VTIME] = 0;
        new_termios.c_cc[VMIN] = 1;
    } else {  // Неканонический режим
        new_termios.c_lflag &= ~ICANON;
        new_termios.c_cc[VTIME] = vtime;
        new_termios.c_cc[VMIN] = vmin;
    }
    
    // Управление эхо-выводом
    if (echo) {
        new_termios.c_lflag |= ECHO;
    } else {
        new_termios.c_lflag &= ~ECHO;
    }
    
    // Управление сигналом SIGINT
    if (sigint) {
        new_termios.c_lflag |= ISIG;
    } else {
        new_termios.c_lflag &= ~ISIG;
    }
    
    if (tcsetattr(STDIN_FILENO, TCSANOW, &new_termios) == -1) {
        return -1;
    }
    
    return 0;
}