#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include "mySimpleComputer.h"
#include "myTerm.h"

// Проверка, является ли поток терминалом
int isTerminal(void) {
    return isatty(STDOUT_FILENO);
}

// Проверка размера экрана
int checkScreenSize(void) {
    int rows, cols;
    if (mt_getscreensize(&rows, &cols) == -1) {
        return -1;
    }
    
    // Минимальные требования: 25 строк и 80 столбцов
    if (rows < 25 || cols < 80) {
        printf("Ошибка: размер терминала должен быть не менее 25x80\n");
        printf("Текущий размер: %dx%d\n", rows, cols);
        return -1;
    }
    return 0;
}

// Вывод рамок и заголовков
void drawUI(void) {
    mt_setfgcolor(COLOR_WHITE);
    mt_setbgcolor(COLOR_BLUE);
    
    // Заголовки блоков
    mt_gotoXY(1, 2);
    printf("=== ОПЕРАТИВНАЯ ПАМЯТЬ ===");
    
    mt_gotoXY(1, 45);
    printf("=== РЕГИСТРЫ ===");
    
    mt_gotoXY(5, 45);
    printf("Аккумулятор:");
    
    mt_gotoXY(7, 45);
    printf("Счетчик команд:");
    
    mt_gotoXY(10, 45);
    printf("Флаги:");
    
    mt_gotoXY(13, 45);
    printf("Команда:");
    
    mt_gotoXY(16, 45);
    printf("Декодированная ячейка:");
    
    mt_gotoXY(19, 2);
    printf("=== ВВОД-ВЫВОД ===");
    
    mt_setdefaultcolor();
}

// Вывод всех ячеек памяти
void printAllMemory(int currentAddress) {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        if (i == currentAddress) {
            sc_printCell(i, COLOR_BLACK, COLOR_WHITE);  // Инверсный режим
        } else {
            sc_printCell(i, COLOR_WHITE, COLOR_BLACK);
        }
    }
}

int main(void) {
    printf("\033[?25l"); // Скрываем курсор
    
    // Проверка терминала
    if (!isTerminal()) {
        printf("Ошибка: программа должна запускаться в терминале\n");
        return 1;
    }
    
    // Проверка размера экрана
    if (checkScreenSize() == -1) {
        return 1;
    }
    
    // Инициализация
    sc_memoryInit();
    sc_accumulatorInit();
    sc_icounterInit();
    sc_regInit();
    
    // Заполнение памяти тестовыми значениями
    sc_memorySet(0, 0x1234);
    sc_memorySet(1, 0x5678);
    sc_memorySet(2, 0x9ABC);
    sc_memorySet(3, 0xDEF0);
    sc_memorySet(10, 0x1122);
    sc_memorySet(11, 0x3344);
    sc_memorySet(12, 0x5566);
    
    // Установка начального счетчика команд
    sc_icounterSet(0);
    
    // Установка флагов для демонстрации
    sc_regSet(REG_FLAG_OVERFLOW, 1);
    sc_regSet(REG_FLAG_MEMORY, 1);
    
    // Установка аккумулятора
    sc_accumulatorSet(0xABCD);
    
    // Очистка экрана и отрисовка интерфейса
    mt_clrscr();
    drawUI();
    
    // Вывод памяти (текущая ячейка - адрес 0)
    printAllMemory(0);
    
    // Вывод регистров
    sc_printFlags();
    sc_printAccumulator();
    sc_printCounters();
    sc_printCommand();
    
    // Декодирование текущей ячейки
    int currentValue;
    sc_memoryGet(0, &currentValue);
    sc_printDecodedCommand(currentValue);
    
    // Вывод в блок IN-OUT (7 значений)
    sc_printTerm(0, 0);
    sc_printTerm(1, 0);
    sc_printTerm(2, 0);
    sc_printTerm(10, 0);
    sc_printTerm(11, 0);
    sc_printTerm(12, 0);
    sc_printTerm(3, 1);  // Ожидание ввода
    
    // Перемещение курсора в нижнюю часть экрана
    mt_gotoXY(24, 1);
    mt_setfgcolor(COLOR_YELLOW);
    printf("Нажмите Enter для выхода...");
    mt_setdefaultcolor();
    
    // Ожидание ввода
    getchar();
    
    // Очистка и возврат курсора
    mt_clrscr();
    mt_gotoXY(1, 1);
    mt_setcursorvisible(1);
    
    return 0;
}