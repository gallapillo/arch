#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Таблица команд
typedef struct {
    char *name;
    int code;
    int has_operand;
} Command;

extern Command commands[];

// Функции транслятора
int assemble_file(const char *input_file, const char *output_file);
int parse_line(char *line, int *address, int *value);
int get_command_code(const char *cmd_name);
int parse_operand(const char *operand);
void add_comment(char *line, char *comment);

#endif