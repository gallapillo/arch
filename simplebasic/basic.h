#ifndef BASIC_H
#define BASIC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Структура для хранения строки программы
typedef struct {
    int line_num;
    char statement[32];
    char params[128];
    struct BasicLine *next;
} BasicLine;

// Функции транслятора
int compile_basic(const char *input_file, const char *output_asm, const char *output_bin);
int parse_basic_line(char *line, BasicLine *basic_line);
int generate_asm(BasicLine *program, const char *output_file);
int evaluate_expression(const char *expr, int *result);

#endif