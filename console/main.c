#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "mySimpleComputer.h"
#include "myTerm.h"
#include "myBigChars.h"
#include "myReadkey.h"

// Глобальные переменные
static int current_address = 0;
static int edit_mode = 0;
static int running = 1;

// Прототипы
int isTerminal(void);
int checkScreenSize(void);
void drawUI(void);
void printAllMemory(void);
void updateDisplay(void);
void handleKey(void);
void editCell(void);
void editAccumulator(void);
void editIcounter(void);

// Внешние функции
extern int sc_loadFont(const char *filename);
extern void sc_printBigCell(int address);

int isTerminal(void) {
    return isatty(STDIN_FILENO) && isatty(STDOUT_FILENO);
}

int checkScreenSize(void) {
    int rows, cols;
    if (mt_getscreensize(&rows, &cols) == -1) {
        return -1;
    }
    
    if (rows < 30 || cols < 100) {
        printf("Ошибка: размер терминала должен быть не менее 30x100\n");
        printf("Текущий размер: %dx%d\n", rows, cols);
        return -1;
    }
    return 0;
}

void drawUI(void) {
    mt_clrscr();
    
    // Очищаем весь экран и выводим рамки простыми символами
    
    // Верхняя граница блока памяти
    mt_setfgcolor(COLOR_WHITE);
    mt_setbgcolor(COLOR_BLACK);
    
    // Заголовок "ОПЕРАТИВНАЯ ПАМЯТЬ"
    mt_gotoXY(1, 15);
    printf("ОПЕРАТИВНАЯ ПАМЯТЬ");
    
    // Заголовок "РЕГИСТРЫ"
    mt_gotoXY(1, 50);
    printf("РЕГИСТРЫ");
    
    // Заголовок "УВЕЛИЧЕННОЕ ЗНАЧЕНИЕ"
    mt_gotoXY(1, 70);
    printf("УВЕЛИЧЕННОЕ ЗНАЧЕНИЕ");
    
    // Рамка памяти (строки 2-17, столбцы 1-46)
    for (int i = 2; i <= 17; i++) {
        mt_gotoXY(i, 1);
        printf("|");
        mt_gotoXY(i, 46);
        printf("|");
    }
    mt_gotoXY(2, 1);
    printf("+");
    mt_gotoXY(2, 46);
    printf("+");
    mt_gotoXY(17, 1);
    printf("+");
    mt_gotoXY(17, 46);
    printf("+");
    
    // Рамка регистров (строки 1-17, столбцы 47-66)
    for (int i = 1; i <= 17; i++) {
        mt_gotoXY(i, 47);
        printf("|");
        mt_gotoXY(i, 66);
        printf("|");
    }
    mt_gotoXY(1, 47);
    printf("+");
    mt_gotoXY(1, 66);
    printf("+");
    mt_gotoXY(17, 47);
    printf("+");
    mt_gotoXY(17, 66);
    printf("+");
    
    // Метки регистров
    mt_gotoXY(3, 49);
    mt_setfgcolor(COLOR_CYAN);
    printf("Аккумулятор:");
    
    mt_gotoXY(5, 49);
    printf("Счетчик команд:");
    
    mt_gotoXY(7, 49);
    printf("Флаги:");
    
    mt_gotoXY(9, 49);
    printf("Команда:");
    
    mt_gotoXY(11, 49);
    printf("Декодированная ячейка:");
    
    mt_setdefaultcolor();
    
    // Рамка IN-OUT (строки 18-24, столбцы 1-46)
    mt_gotoXY(18, 1);
    printf("+");
    for (int i = 2; i <= 46; i++) printf("-");
    printf("+");
    
    for (int i = 19; i <= 23; i++) {
        mt_gotoXY(i, 1);
        printf("|");
        mt_gotoXY(i, 46);
        printf("|");
    }
    
    mt_gotoXY(24, 1);
    printf("+");
    for (int i = 2; i <= 46; i++) printf("-");
    printf("+");
    
    mt_gotoXY(18, 20);
    printf("IN-OUT");
    
    // Подсказки
    mt_gotoXY(26, 2);
    mt_setfgcolor(COLOR_YELLOW);
    printf("Управление: стрелки - навигация, Enter - редактировать ячейку");
    mt_gotoXY(27, 2);
    printf("F5 - редактировать аккумулятор, F6 - редактировать счетчик команд");
    mt_gotoXY(28, 2);
    printf("l - загрузить память, s - сохранить память, i - инициализировать");
    mt_gotoXY(29, 2);
    printf("ESC - выход");
    mt_setdefaultcolor();
    
    fflush(stdout);
}

void printAllMemory(void) {
    // Сначала очищаем всю область памяти
    for (int row = 0; row < 13; row++) {
        mt_gotoXY(MEMORY_START_ROW + row, MEMORY_START_COL);
        mt_delline();
        // Дополнительно очищаем длинной строкой пробелов
        for (int i = 0; i < 60; i++) {
            printf(" ");
        }
    }
    
    // Теперь выводим все ячейки
    for (int i = 0; i < MEMORY_SIZE; i++) {
        if (i == current_address && !edit_mode) {
            sc_printCell(i, COLOR_BLACK, COLOR_WHITE);  // Инверсный режим
        } else if (i == current_address && edit_mode) {
            sc_printCell(i, COLOR_YELLOW, COLOR_RED);   // Режим редактирования
        } else {
            sc_printCell(i, COLOR_WHITE, COLOR_BLACK);
        }
    }
    
    fflush(stdout);
}

void updateDisplay(void) {
    // Очищаем область памяти
    for (int row = 0; row < 13; row++) {
        mt_gotoXY(MEMORY_START_ROW + row, MEMORY_START_COL);
        for (int col = 0; col < 60; col++) {
            printf(" ");
        }
    }
    
    // Вывод памяти
    printAllMemory();
    
    // Вывод регистров
    sc_printFlags();
    sc_printAccumulator();
    sc_printCounters();
    sc_printCommand();
    
    // Декодирование текущей ячейки
    int currentValue;
    sc_memoryGet(current_address, &currentValue);
    sc_printDecodedCommand(currentValue);
    
    // Вывод большой ячейки
    sc_printBigCell(current_address);
    
    // Обновление курсора
    if (!edit_mode) {
        int row = MEMORY_START_ROW + current_address / MEMORY_COLS;
        int col = MEMORY_START_COL + (current_address % MEMORY_COLS) * 6;
        mt_gotoXY(row, col);
    }
    
    fflush(stdout);
}

void moveCursor(int direction) {
    if (edit_mode) return;
    
    switch (direction) {
        case KEY_UP:
            current_address -= 10;
            break;
        case KEY_DOWN:
            current_address += 10;
            break;
        case KEY_LEFT:
            current_address--;
            break;
        case KEY_RIGHT:
            current_address++;
            break;
    }
    
    // Циклическое перемещение
    if (current_address < 0) current_address = MEMORY_SIZE - 1;
    if (current_address >= MEMORY_SIZE) current_address = 0;
    
    updateDisplay();
}

void editCell(void) {
    edit_mode = 1;
    
    // Получаем текущее значение
    int old_value, new_value;
    sc_memoryGet(current_address, &old_value);
    
    // Выводим ячейку в режиме редактирования
    sc_printCell(current_address, COLOR_YELLOW, COLOR_RED);
    
    // Перемещаем курсор в позицию для ввода (после знака)
    int row = MEMORY_START_ROW + current_address / MEMORY_COLS;
    int col = MEMORY_START_COL + (current_address % MEMORY_COLS) * 6 + 5;
    mt_gotoXY(row, col);
    
    // Ввод нового значения
    int result = rk_readvalue(&new_value, -1);  // Без таймаута
    
    if (result == 0 && new_value != old_value) {
        sc_memorySet(current_address, new_value);
    }
    
    edit_mode = 0;
    updateDisplay();
}

void editAccumulator(void) {
    int old_value, new_value;
    sc_accumulatorGet(&old_value);
    
    // Позиция для редактирования
    mt_gotoXY(3, 60);
    mt_setfgcolor(COLOR_YELLOW);
    mt_setbgcolor(COLOR_RED);
    printf("%04X    ", old_value & 0xFFFF);
    mt_gotoXY(3, 60);
    
    int result = rk_readvalue(&new_value, -1);
    
    mt_setdefaultcolor();
    if (result == 0) {
        sc_accumulatorSet(new_value);
    }
    
    updateDisplay();
}

void editIcounter(void) {
    int old_value, new_value;
    sc_icounterGet(&old_value);
    
    // Позиция для редактирования
    mt_gotoXY(5, 62);
    mt_setfgcolor(COLOR_YELLOW);
    mt_setbgcolor(COLOR_RED);
    printf("%04X    ", old_value & 0xFFFF);
    mt_gotoXY(5, 62);
    
    int result = rk_readvalue(&new_value, -1);
    
    mt_setdefaultcolor();
    if (result == 0 && new_value >= 0 && new_value < MEMORY_SIZE) {
        sc_icounterSet(new_value);
    }
    
    updateDisplay();
}

void saveMemory(void) {
    sc_memorySave("memory.bin");
    
    mt_gotoXY(20, 5);
    mt_setfgcolor(COLOR_GREEN);
    printf("Память сохранена в memory.bin");
    mt_setdefaultcolor();
    usleep(500000);
    
    // Очищаем сообщение
    mt_gotoXY(20, 5);
    printf("                           ");
    updateDisplay();
}

void loadMemory(void) {
    sc_memoryLoad("memory.bin");
    
    mt_gotoXY(20, 5);
    mt_setfgcolor(COLOR_GREEN);
    printf("Память загружена из memory.bin");
    mt_setdefaultcolor();
    usleep(500000);
    
    mt_gotoXY(20, 5);
    printf("                           ");
    updateDisplay();
}

void initAll(void) {
    sc_memoryInit();
    sc_accumulatorInit();
    sc_icounterInit();
    sc_regInit();
    current_address = 0;
    updateDisplay();
    
    mt_gotoXY(20, 5);
    mt_setfgcolor(COLOR_GREEN);
    printf("Все регистры и память инициализированы");
    mt_setdefaultcolor();
    usleep(500000);
    
    mt_gotoXY(20, 5);
    printf("                                    ");
}

void handleKey(void) {
    enum keys key;
    
    if (rk_readkey(&key) == -1) {
        return;
    }
    
    switch (key) {
        case KEY_UP:
        case KEY_DOWN:
        case KEY_LEFT:
        case KEY_RIGHT:
            moveCursor(key);
            break;
            
        case KEY_ENTER:
            if (!edit_mode) {
                editCell();
            }
            break;
            
        case KEY_ESC:
            running = 0;
            break;
            
        case KEY_F5:
            editAccumulator();
            break;
            
        case KEY_F6:
            editIcounter();
            break;
            
        case KEY_L:
            loadMemory();
            break;
            
        case KEY_S:
            saveMemory();
            break;
            
        case KEY_I:
            initAll();
            break;
            
        default:
            break;
    }
}

void cleanup(void) {
    mt_clrscr();
    mt_gotoXY(1, 1);
    mt_setcursorvisible(1);
    rk_mytermrestore();
}

void signal_handler(int signo) {
    if (signo == SIGINT) {
        cleanup();
        exit(0);
    }
}

int main(int argc, char *argv[]) {
    const char *font_file = "font.bin";
    
    if (argc > 1) {
        font_file = argv[1];
    }
    
    // Настройка обработчика сигналов
    signal(SIGINT, signal_handler);
    
    // Проверка терминала
    if (!isTerminal()) {
        printf("Ошибка: программа должна запускаться в терминале\n");
        return 1;
    }
    
    // Проверка размера экрана
    if (checkScreenSize() == -1) {
        return 1;
    }
    
    // Сохраняем настройки терминала
    if (rk_mytermsave() == -1) {
        printf("Ошибка: не удалось сохранить настройки терминала\n");
        return 1;
    }
    
    // Переключаем в неканонический режим
    if (rk_mytermregime(0, 0, 1, 0, 0) == -1) {
        printf("Ошибка: не удалось переключить режим терминала\n");
        return 1;
    }
    
    // Загрузка шрифта
    if (sc_loadFont(font_file) == -1) {
        mt_clrscr();
        mt_gotoXY(1, 1);
        printf("Ошибка: не удалось загрузить файл шрифта '%s'\n", font_file);
        printf("Запустите программу font для генерации шрифта\n");
        cleanup();
        return 1;
    }
    
    // Инициализация
    sc_memoryInit();
    sc_accumulatorInit();
    sc_icounterInit();
    sc_regInit();
    
    // Тестовые значения
    sc_memorySet(0, 0x1234);
    sc_memorySet(1, 0x5678);
    sc_memorySet(2, 0x9ABC);
    sc_memorySet(3, 0xDEF0);
    sc_memorySet(10, 0x1122);
    sc_memorySet(11, 0x3344);
    sc_memorySet(12, 0x5566);
    
    sc_accumulatorSet(0xABCD);
    sc_regSet(REG_FLAG_OVERFLOW, 1);
    sc_regSet(REG_FLAG_MEMORY, 1);
    
    // Скрываем курсор
    mt_setcursorvisible(0);
    
    // Отрисовка интерфейса
    drawUI();
    updateDisplay();
    
    // Вывод в блок IN-OUT
    sc_printTerm(0, 0);
    sc_printTerm(1, 0);
    sc_printTerm(2, 0);
    sc_printTerm(10, 0);
    sc_printTerm(11, 0);
    sc_printTerm(12, 0);
    
    // Основной цикл
    while (running) {
        handleKey();
    }
    
    // Очистка и выход
    cleanup();
    
    return 0;
}