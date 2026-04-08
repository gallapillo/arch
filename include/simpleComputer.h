#ifndef SIMPLECOMPUTER_H
#define SIMPLECOMPUTER_H

#include "mySimpleComputer.h"

// Структура строки кэша
typedef struct {
    int valid;      // Флаг валидности
    int tag;        // Тег (старшие биты адреса)
    int data;       // Данные
    int last_used;  // Для алгоритма LRU
} CacheLine;

// Структура кэша
typedef struct {
    CacheLine lines[8];  // 8 строк кэша
    int hits;            // Количество попаданий
    int misses;          // Количество промахов
    int access_count;    // Общее количество обращений
    int lru_counter;     // Счетчик для LRU
} Cache;

// Функции кэша
int cache_init(void);
int cache_read(int address, int *value);
int cache_write(int address, int value);
int cache_get_hits(void);
int cache_get_misses(void);
void cache_print_stats(void);

// Функции обработчика прерываний
int interrupt_init(void);
int interrupt_request(int vector);
int interrupt_handle(void);
int interrupt_enable(int enable);

#endif