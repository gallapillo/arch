#include "myBigChars.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int bc_box(int x1, int y1, int x2, int y2,
           enum colors box_fg, enum colors box_bg,
           char *header, enum colors header_fg, enum colors header_bg) {
    
    if (x1 >= x2 || y1 >= y2) {
        return -1;
    }
    
    int height = x2 - x1 + 1;
    int width = y2 - y1 + 1;
    
    // Устанавливаем цвета для рамки
    mt_setfgcolor(box_fg);
    mt_setbgcolor(box_bg);
    
    // Вход в режим псевдографики
    write(STDOUT_FILENO, "\033(0", 3);
    
    // Верхняя граница
    mt_gotoXY(x1, y1);
    write(STDOUT_FILENO, "l", 1);  // левый верхний угол
    for (int i = 1; i < width - 1; i++) {
        write(STDOUT_FILENO, "q", 1);  // горизонталь
    }
    write(STDOUT_FILENO, "k", 1);  // правый верхний угол
    
    // Боковые границы
    for (int i = 1; i < height - 1; i++) {
        mt_gotoXY(x1 + i, y1);
        write(STDOUT_FILENO, "x", 1);  // вертикаль
        mt_gotoXY(x1 + i, y1 + width - 1);
        write(STDOUT_FILENO, "x", 1);  // вертикаль
    }
    
    // Нижняя граница
    mt_gotoXY(x1 + height - 1, y1);
    write(STDOUT_FILENO, "m", 1);  // левый нижний угол
    for (int i = 1; i < width - 1; i++) {
        write(STDOUT_FILENO, "q", 1);  // горизонталь
    }
    write(STDOUT_FILENO, "j", 1);  // правый нижний угол
    
    // Выход из режима псевдографики
    write(STDOUT_FILENO, "\033(B", 3);
    
    // Вывод заголовка
    if (header != NULL && bc_strlen(header) > 0) {
        int header_len = bc_strlen(header);
        int start_col = y1 + (width - header_len) / 2;
        
        mt_gotoXY(x1, start_col);
        mt_setfgcolor(header_fg);
        mt_setbgcolor(header_bg);
        printf("%s", header);
    }
    
    mt_setdefaultcolor();
    return 0;
}