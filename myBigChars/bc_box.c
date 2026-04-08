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
    
    // Используем UTF-8 псевдографику (более совместимо с macOS)
    // Верхняя граница
    mt_gotoXY(x1, y1);
    printf("┌");
    for (int i = 1; i < width - 1; i++) {
        printf("─");
    }
    printf("┐");
    
    // Боковые границы
    for (int i = 1; i < height - 1; i++) {
        mt_gotoXY(x1 + i, y1);
        printf("│");
        mt_gotoXY(x1 + i, y1 + width - 1);
        printf("│");
    }
    
    // Нижняя граница
    mt_gotoXY(x1 + height - 1, y1);
    printf("└");
    for (int i = 1; i < width - 1; i++) {
        printf("─");
    }
    printf("┘");
    
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