#ifndef MYREADKEY_H
#define MYREADKEY_H

#include <termios.h>

// Перечисление распознаваемых клавиш
enum keys {
    KEY_NONE = 0,
    KEY_UP,
    KEY_DOWN,
    KEY_LEFT,
    KEY_RIGHT,
    KEY_ENTER,
    KEY_ESC,
    KEY_F5,
    KEY_F6,
    KEY_L,
    KEY_S,
    KEY_I,
    KEY_PLUS,
    KEY_MINUS,
    KEY_0, KEY_1, KEY_2, KEY_3, KEY_4,
    KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,
    KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F
};

// Функции библиотеки myReadkey
int rk_readkey(enum keys *key);
int rk_mytermsave(void);
int rk_mytermrestore(void);
int rk_mytermregime(int regime, int vtime, int vmin, int echo, int sigint);
int rk_readvalue(int *value, int timeout);

// Глобальные переменные
extern struct termios saved_termios;

#endif