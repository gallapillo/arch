#include "sc.h"
#include "myBigChars.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

static int font[FONT_SIZE][2];
static int font_loaded = 0;

// Загрузка шрифта из файла
int sc_loadFont(const char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        return -1;
    }
    
    int count;
    if (bc_bigcharread(fd, (int *)font, FONT_SIZE, &count) == -1 || count != FONT_SIZE) {
        close(fd);
        return -1;
    }
    
    close(fd);
    font_loaded = 1;
    return 0;
}

void sc_printBigCell(int address) {
    if (!font_loaded) {
        return;
    }
    
    int value;
    sc_memoryGet(address, &value);
    
    int hex_digit = (value >> 12) & 0x0F;
    int index;
    
    // Преобразование hex в индекс шрифта
    if (hex_digit >= 0 && hex_digit <= 9) {
        index = hex_digit;
    } else if (hex_digit == 0xA) {
        index = 10; // A
    } else if (hex_digit == 0xB) {
        index = 11; // B
    } else if (hex_digit == 0xC) {
        index = 12; // C
    } else if (hex_digit == 0xD) {
        index = 13; // D
    } else if (hex_digit == 0xE) {
        index = 14; // E
    } else if (hex_digit == 0xF) {
        index = 15; // F
    } else {
        return;
    }
    
    // Вывод большого символа в правой части экрана
    bc_printbigchar(font[index], 5, 65, COLOR_CYAN, COLOR_BLACK);
}