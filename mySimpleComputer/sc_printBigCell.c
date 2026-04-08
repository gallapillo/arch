#include "sc.h"
#include "myBigChars.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

static int font[FONT_SIZE][2];
static int font_loaded = 0;

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
    
    if (hex_digit >= 0 && hex_digit <= 9) {
        index = hex_digit;
    } else if (hex_digit >= 0xA && hex_digit <= 0xF) {
        index = 10 + (hex_digit - 0xA);
    } else {
        return;
    }
    
    bc_printbigchar(font[index], 5, 67, COLOR_CYAN, COLOR_BLACK);
}