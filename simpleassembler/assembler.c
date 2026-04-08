#include "assembler.h"
#include "../include/mySimpleComputer.h"

// Таблица команд Simple Assembler
Command commands[] = {
    {"READ", 0x10, 1},
    {"WRITE", 0x11, 1},
    {"LOAD", 0x20, 1},
    {"STORE", 0x21, 1},
    {"ADD", 0x30, 1},
    {"SUB", 0x31, 1},
    {"DIVIDE", 0x32, 1},
    {"MUL", 0x33, 1},
    {"JUMP", 0x40, 1},
    {"JNEG", 0x41, 1},
    {"JZ", 0x42, 1},
    {"HALT", 0x00, 0},
    {"=", 0xFF, 1},  // Прямое задание значения
    {NULL, -1, 0}
};

int get_command_code(const char *cmd_name) {
    for (int i = 0; commands[i].name != NULL; i++) {
        if (strcmp(commands[i].name, cmd_name) == 0) {
            return commands[i].code;
        }
    }
    return -1;
}

int parse_operand(const char *operand) {
    if (operand == NULL || strlen(operand) == 0) {
        return 0;
    }
    
    // Пропускаем пробелы
    while (isspace(*operand)) operand++;
    
    // Проверка на знак
    int sign = 0;
    if (*operand == '+') {
        sign = 0;
        operand++;
    } else if (*operand == '-') {
        sign = 1;
        operand++;
    }
    
    // Парсим число
    int value = 0;
    while (isdigit(*operand)) {
        value = value * 10 + (*operand - '0');
        operand++;
    }
    
    // Кодируем значение с учетом знака
    if (sign) {
        value = -value;
    }
    
    return value & 0x7FFF;
}

int parse_line(char *line, int *address, int *value) {
    char *token;
    char cmd[32];
    char operand[32];
    int addr;
    
    // Удаляем комментарий
    char *comment = strchr(line, ';');
    if (comment) {
        *comment = '\0';
    }
    
    // Парсим адрес
    token = strtok(line, " \t\n\r");
    if (token == NULL) return -1;
    addr = atoi(token);
    
    // Парсим команду
    token = strtok(NULL, " \t\n\r");
    if (token == NULL) return -1;
    strcpy(cmd, token);
    
    // Парсим операнд
    token = strtok(NULL, " \t\n\r");
    if (token == NULL) {
        operand[0] = '\0';
    } else {
        strcpy(operand, token);
    }
    
    int cmd_code = get_command_code(cmd);
    if (cmd_code == -1) {
        fprintf(stderr, "Ошибка: неизвестная команда '%s'\n", cmd);
        return -1;
    }
    
    int operand_value = 0;
    if (commands[cmd_code].has_operand || cmd_code == 0xFF) {
        operand_value = parse_operand(operand);
    }
    
    // Кодируем команду
    int sign = 0;
    if (cmd_code == 0xFF) {
        // Прямое значение
        *value = operand_value;
    } else {
        sc_commandEncode(0, cmd_code, operand_value, value);
    }
    
    *address = addr;
    return 0;
}

int assemble_file(const char *input_file, const char *output_file) {
    FILE *in = fopen(input_file, "r");
    if (!in) {
        fprintf(stderr, "Ошибка: не удалось открыть файл %s\n", input_file);
        return -1;
    }
    
    // Инициализируем память
    sc_memoryInit();
    
    char line[256];
    int line_num = 0;
    
    while (fgets(line, sizeof(line), in)) {
        line_num++;
        
        // Пропускаем пустые строки
        if (line[0] == '\n' || line[0] == '\r') continue;
        
        int address, value;
        if (parse_line(line, &address, &value) == -1) {
            fprintf(stderr, "Ошибка в строке %d: %s", line_num, line);
            fclose(in);
            return -1;
        }
        
        if (sc_memorySet(address, value) == -1) {
            fprintf(stderr, "Ошибка: неверный адрес %d в строке %d\n", address, line_num);
            fclose(in);
            return -1;
        }
    }
    
    fclose(in);
    
    // Сохраняем бинарный файл
    if (sc_memorySave(output_file) == -1) {
        fprintf(stderr, "Ошибка: не удалось сохранить файл %s\n", output_file);
        return -1;
    }
    
    printf("Трансляция завершена. Результат сохранен в %s\n", output_file);
    return 0;
}