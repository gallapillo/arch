#include <stdio.h>
#include "basic.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Использование: %s <input.sb> [output.asm] [output.bin]\n", argv[0]);
        fprintf(stderr, "Пример: %s program.sb program.asm program.o\n", argv[0]);
        return 1;
    }
    
    const char *input_file = argv[1];
    const char *output_asm = "output.asm";
    const char *output_bin = "output.o";
    
    if (argc >= 3) output_asm = argv[2];
    if (argc >= 4) output_bin = argv[3];
    
    if (compile_basic(input_file, output_asm, output_bin) == -1) {
        return 1;
    }
    
    // Запуск ассемблера
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "./../simpleassembler/sat %s %s", output_asm, output_bin);
    printf("Запуск ассемблера: %s\n", cmd);
    system(cmd);
    
    return 0;
}