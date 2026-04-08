#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
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
void saveMemory(void);
void loadMemory(void);
void initAll(void);

// Внешние функции
extern int sc_loadFont(const char *filename);
extern void sc_printBigCell(int address);

int isTerminal(void) {
    return isatty(STDIN_FILENO) && isatty(STDOUT_FILENO);
}

int checkScreenSize(void) {
    int rows, cols;
    if (mt_getscreensize(&rows, &cols) == -1) {
        // Если не удалось получить размер, используем значения по умолчанию
        rows = 30;
        cols = 100;
    }
    
    if (rows < 25 || cols < 80) {
        printf("Ошибка: размер терминала должен быть не менее 25x80\n");
        printf("Текущий размер: %dx%d\n", rows, cols);
        return -1;
    }
    return 0;
}

void drawUI(void) {
    mt_clrscr();
    
    // Заголовки
    mt_setfgcolor(COLOR_WHITE);
    mt_setbgcolor(COLOR_BLUE);
    
    mt_gotoXY(1, 2);
    printf("=== ОПЕРАТИВНАЯ ПАМЯТЬ ===");
    
    mt_gotoXY(1, 45);
    printf("=== РЕГИСТРЫ ===");
    
    mt_gotoXY(1, 65);
    printf("=== УВЕЛИЧЕННОЕ ЗНАЧЕНИЕ ===");
    
    mt_setdefaultcolor();
    
    // Псевдографические рамки
    bc_box(2, 1, 18, 44, COLOR_WHITE, COLOR_BLACK, NULL, COLOR_WHITE, COLOR_BLACK);
    bc_box(1, 45, 18, 64, COLOR_WHITE, COLOR_BLACK, NULL, COLOR_WHITE, COLOR_BLACK);
    bc_box(4, 65, 12, 95, COLOR_WHITE, COLOR_BLACK, "BIG CHAR", COLOR_YELLOW, COLOR_BLACK);
    bc_box(19, 1, 24, 95, COLOR_WHITE, COLOR_BLACK, "IN-OUT", COLOR_YELLOW, COLOR_BLACK);
    
    // Метки регистров
    mt_gotoXY(3, 47);
    mt_setfgcolor(COLOR_CYAN);
    printf("Аккумулятор:");
    
    mt_gotoXY(5, 47);
    printf("Счетчик команд:");
    
    mt_gotoXY(7, 47);
    printf("Флаги:");
    
    mt_gotoXY(9, 47);
    printf("Команда:");
    
    mt_gotoXY(11, 47);
    printf("Декодированная ячейка:");
    mt_setdefaultcolor();
    
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
}

void printAllMemory(void) {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        if (i == current_address && !edit_mode) {
            sc_printCell(i, COLOR_BLACK, COLOR_WHITE);  // Инверсный режим
        } else if (i == current_address && edit_mode) {
            sc_printCell(i, COLOR_YELLOW, COLOR_RED);   // Режим редактирования
        } else {
            sc_printCell(i, COLOR_WHITE, COLOR_BLACK);
        }
    }
}

void updateDisplay(void) {
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
    
    // Обновление курсора в позиции текущей ячейки
    if (!edit_mode) {
        int row = MEMORY_START_ROW + current_address / MEMORY_COLS;
        int col = MEMORY_START_COL + (current_address % MEMORY_COLS) * 6;
        mt_gotoXY(row, col);
    }
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
    if (sc_memorySave("memory.bin") == 0) {
        mt_gotoXY(20, 5);
        mt_setfgcolor(COLOR_GREEN);
        printf("Память сохранена в memory.bin");
        mt_setdefaultcolor();
        usleep(500000);
    }
    
    // Очищаем сообщение
    mt_gotoXY(20, 5);
    printf("                           ");
    updateDisplay();
}

void loadMemory(void) {
    if (sc_memoryLoad("memory.bin") == 0) {
        mt_gotoXY(20, 5);
        mt_setfgcolor(COLOR_GREEN);
        printf("Память загружена из memory.bin");
        mt_setdefaultcolor();
        usleep(500000);
    }
    
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