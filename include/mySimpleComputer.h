#ifndef MYSIMPLECOMPUTER_H
#define MYSIMPLECOMPUTER_H

#include "myTerm.h"

#define MEMORY_SIZE 128

// Маски для регистров флагов
#define FLAG_OVERFLOW     0x01
#define FLAG_DIVISION     0x02
#define FLAG_MEMORY       0x04
#define FLAG_IGNORE       0x08
#define FLAG_HALT         0x10

#define REG_FLAG_OVERFLOW   0
#define REG_FLAG_DIVISION   1
#define REG_FLAG_MEMORY     2
#define REG_FLAG_IGNORE     3
#define REG_FLAG_HALT       4

#define MAX_COMMAND_VALUE   0x3F
#define MAX_OPERAND_VALUE   0x7F

// Функции работы с памятью
int sc_memoryInit(void);
int sc_memorySet(int address, int value);
int sc_memoryGet(int address, int *value);
int sc_memorySave(char *filename);
int sc_memoryLoad(char *filename);

// Функции работы с регистрами флагов
int sc_regInit(void);
int sc_regSet(int reg, int value);
int sc_regGet(int reg, int *value);

// Функции работы с аккумулятором
int sc_accumulatorInit(void);
int sc_accumulatorSet(int value);
int sc_accumulatorGet(int *value);

// Функции работы со счетчиком команд
int sc_icounterInit(void);
int sc_icounterSet(int value);
int sc_icounterGet(int *value);

// Функции кодирования/декодирования команд
int sc_commandEncode(int sign, int command, int operand, int *value);
int sc_commandDecode(int value, int *sign, int *command, int *operand);
int sc_commandValidate(int command);

// Функции вывода
void sc_printCell(int address, enum colors fg, enum colors bg);
void sc_printFlags(void);
void sc_printDecodedCommand(int value);
void sc_printAccumulator(void);
void sc_printCounters(void);
void sc_printTerm(int address, int isInput);
void sc_printCommand(void);
void sc_printBigCell(int address);

// Функции загрузки шрифта
int sc_loadFont(const char *filename);

// Константы для расположения блоков
#define MEMORY_START_ROW    3
#define MEMORY_START_COL    3
#define MEMORY_COLS         10
#define FLAGS_ROW           7
#define FLAGS_COL           55
#define ACCUM_ROW           3
#define ACCUM_COL           55
#define ICOUNTER_ROW        5
#define ICOUNTER_COL        55
#define COMMAND_ROW         9
#define COMMAND_COL         55
#define DECODED_ROW         11
#define DECODED_COL         55
#define TERM_START_ROW      20
#define TERM_START_COL      4
#define TERM_LINES          4

#endif