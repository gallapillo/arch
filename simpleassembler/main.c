#include <stdio.h>
#include "assembler.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Использование: %s <input.sa> <output.o>\n", argv[0]);
        fprintf(stderr, "Пример: %s program.sa program.o\n", argv[0]);
        return 1;
    }
    
    const char *input_file = argv[1];
    const char *output_file = argv[2];
    
    if (assemble_file(input_file, output_file) == -1) {
        return 1;
    }
    
    return 0;
}