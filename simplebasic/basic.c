#include "basic.h"
#include <stdarg.h>

// Таблица переменных (A-Z)
static int variables[26];

// Функции для работы с выражениями
int get_priority(char op) {
    switch (op) {
        case '*': case '/': return 2;
        case '+': case '-': return 1;
        default: return 0;
    }
}

int apply_operator(int a, int b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        default: return 0;
    }
}

int evaluate_expression(const char *expr, int *result) {
    int values[256];
    char ops[256];
    int vtop = 0, otop = 0;
    
    const char *p = expr;
    while (*p) {
        if (isspace(*p)) {
            p++;
            continue;
        }
        
        if (isalpha(*p)) {
            // Переменная
            int idx = toupper(*p) - 'A';
            values[vtop++] = variables[idx];
            p++;
        } else if (isdigit(*p) || (*p == '-' && isdigit(*(p+1)))) {
            // Число
            int sign = 1;
            if (*p == '-') {
                sign = -1;
                p++;
            }
            int num = 0;
            while (isdigit(*p)) {
                num = num * 10 + (*p - '0');
                p++;
            }
            values[vtop++] = num * sign;
        } else if (*p == '(') {
            ops[otop++] = '(';
            p++;
        } else if (*p == ')') {
            while (otop > 0 && ops[otop-1] != '(') {
                int b = values[--vtop];
                int a = values[--vtop];
                char op = ops[--otop];
                values[vtop++] = apply_operator(a, b, op);
            }
            otop--; // Убираем '('
            p++;
        } else if (strchr("+-*/", *p)) {
            while (otop > 0 && ops[otop-1] != '(' && 
                   get_priority(ops[otop-1]) >= get_priority(*p)) {
                int b = values[--vtop];
                int a = values[--vtop];
                char op = ops[--otop];
                values[vtop++] = apply_operator(a, b, op);
            }
            ops[otop++] = *p;
            p++;
        } else {
            return -1;
        }
    }
    
    while (otop > 0) {
        int b = values[--vtop];
        int a = values[--vtop];
        char op = ops[--otop];
        values[vtop++] = apply_operator(a, b, op);
    }
    
    *result = values[0];
    return 0;
}

int parse_basic_line(char *line, BasicLine *basic_line) {
    char *token;
    
    // Парсим номер строки
    token = strtok(line, " \t\n\r");
    if (token == NULL) return -1;
    basic_line->line_num = atoi(token);
    
    // Парсим оператор
    token = strtok(NULL, " \t\n\r");
    if (token == NULL) return -1;
    strcpy(basic_line->statement, token);
    
    // Парсим параметры (остаток строки)
    char *params = strtok(NULL, "");
    if (params) {
        strcpy(basic_line->params, params);
    } else {
        basic_line->params[0] = '\0';
    }
    
    return 0;
}

int generate_asm(BasicLine *program, const char *output_file) {
    FILE *out = fopen(output_file, "w");
    if (!out) return -1;
    
    int var_addr[26] = {0};
    int next_var_addr = 0x50;  // Переменные с адреса 0x50
    
    BasicLine *current = program;
    int asm_line = 0;
    
    while (current) {
        fprintf(out, "%02X ", asm_line);
        
        if (strcmp(current->statement, "REM") == 0) {
            fprintf(out, "; %s\n", current->params);
            
        } else if (strcmp(current->statement, "INPUT") == 0) {
            char var = toupper(current->params[0]);
            int addr = var_addr[var - 'A'];
            if (addr == 0) {
                addr = next_var_addr++;
                var_addr[var - 'A'] = addr;
                fprintf(out, "= +%04X ; Переменная %c\n", addr, var);
                asm_line++;
                fprintf(out, "%02X ", asm_line);
            }
            fprintf(out, "READ %02X ; Ввод %c\n", addr, var);
            
        } else if (strcmp(current->statement, "OUTPUT") == 0) {
            char var = toupper(current->params[0]);
            int addr = var_addr[var - 'A'];
            if (addr == 0) {
                addr = next_var_addr++;
                var_addr[var - 'A'] = addr;
                fprintf(out, "= +%04X ; Переменная %c\n", addr, var);
                asm_line++;
                fprintf(out, "%02X ", asm_line);
            }
            fprintf(out, "WRITE %02X ; Вывод %c\n", addr, var);
            
        } else if (strcmp(current->statement, "GOTO") == 0) {
            int target = atoi(current->params);
            fprintf(out, "JUMP %02X ; Переход на %d\n", target, target);
            
        } else if (strcmp(current->statement, "IF") == 0) {
            // Парсим условие: IF C < 0 GOTO 20
            char var;
            char op[4];
            int value;
            char goto_str[8];
            int target;
            
            sscanf(current->params, "%c %3s %d %7s %d", 
                   &var, op, &value, goto_str, &target);
            
            int addr = var_addr[toupper(var) - 'A'];
            if (addr == 0) {
                addr = next_var_addr++;
                var_addr[toupper(var) - 'A'] = addr;
            }
            
            fprintf(out, "LOAD %02X ; Загрузка %c\n", addr, var);
            asm_line++;
            fprintf(out, "%02X ", asm_line);
            fprintf(out, "SUB +%04X ; Вычитание %d\n", value, value);
            asm_line++;
            fprintf(out, "%02X ", asm_line);
            
            if (strcmp(op, "<") == 0) {
                fprintf(out, "JNEG %02X ; Переход если отрицательное\n", target);
            } else if (strcmp(op, "=") == 0) {
                fprintf(out, "JZ %02X ; Переход если равно\n", target);
            }
            
        } else if (strcmp(current->statement, "LET") == 0) {
            // LET C = A - B
            char var;
            char equals;
            char expr[128];
            
            sscanf(current->params, "%c %c %[^\n]", &var, &equals, expr);
            
            int result;
            if (evaluate_expression(expr, &result) == 0) {
                int addr = var_addr[toupper(var) - 'A'];
                if (addr == 0) {
                    addr = next_var_addr++;
                    var_addr[toupper(var) - 'A'] = addr;
                }
                fprintf(out, "LOAD +%04X ; Загрузка значения %d\n", result, result);
                asm_line++;
                fprintf(out, "%02X ", asm_line);
                fprintf(out, "STORE %02X ; Сохранение в %c\n", addr, var);
            } else {
                // Сложное выражение
                // TODO: генерация кода для сложных выражений
                fprintf(out, "; Сложное выражение: %s\n", expr);
            }
            
        } else if (strcmp(current->statement, "END") == 0) {
            fprintf(out, "HALT 00 ; Конец программы\n");
            
        } else {
            fprintf(out, "; Неизвестный оператор: %s\n", current->statement);
        }
        
        asm_line++;
        current = current->next;
    }
    
    fclose(out);
    return 0;
}

int compile_basic(const char *input_file, const char *output_asm, const char *output_bin) {
    FILE *in = fopen(input_file, "r");
    if (!in) {
        fprintf(stderr, "Ошибка: не удалось открыть файл %s\n", input_file);
        return -1;
    }
    
    // Сброс переменных
    memset(variables, 0, sizeof(variables));
    
    // Чтение и парсинг строк
    BasicLine *head = NULL;
    BasicLine *tail = NULL;
    char line[256];
    int line_num = 0;
    
    while (fgets(line, sizeof(line), in)) {
        line_num++;
        
        if (line[0] == '\n' || line[0] == '\r') continue;
        
        BasicLine *new_line = malloc(sizeof(BasicLine));
        if (parse_basic_line(line, new_line) == -1) {
            fprintf(stderr, "Ошибка в строке %d: %s", line_num, line);
            fclose(in);
            return -1;
        }
        
        if (head == NULL) {
            head = new_line;
            tail = new_line;
        } else {
            tail->next = new_line;
            tail = new_line;
        }
    }
    
    fclose(in);
    
    // Генерация ASM
    if (generate_asm(head, output_asm) == -1) {
        fprintf(stderr, "Ошибка: не удалось сгенерировать ASM файл\n");
        return -1;
    }
    
    printf("Трансляция Basic завершена. ASM файл: %s\n", output_asm);
    
    // Освобождение памяти
    BasicLine *current = head;
    while (current) {
        BasicLine *next = current->next;
        free(current);
        current = next;
    }
    
    return 0;
}