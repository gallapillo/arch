#include "sc.h"
#include <stdio.h>

int sc_memoryLoad(char *filename) {
    if (filename == NULL) {
        return -1;
    }
    
    // Сохраняем резервную копию
    int backup[MEMORY_SIZE];
    for (int i = 0; i < MEMORY_SIZE; i++) {
        backup[i] = memory[i];
    }
    
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        return -1;
    }
    
    size_t read = fread(memory, sizeof(int), MEMORY_SIZE, file);
    fclose(file);
    
    if (read != MEMORY_SIZE) {
        // Восстанавливаем из резервной копии
        for (int i = 0; i < MEMORY_SIZE; i++) {
            memory[i] = backup[i];
        }
        return -1;
    }
    
    return 0;
}