#include "../include/simpleComputer.h"
#include <stdio.h>

static Cache cache;
static int interrupt_enabled = 1;
static int pending_interrupt = -1;

int cache_init(void) {
    for (int i = 0; i < 8; i++) {
        cache.lines[i].valid = 0;
        cache.lines[i].tag = 0;
        cache.lines[i].data = 0;
        cache.lines[i].last_used = 0;
    }
    cache.hits = 0;
    cache.misses = 0;
    cache.access_count = 0;
    cache.lru_counter = 0;
    return 0;
}

int cache_get_index(int address) {
    return (address >> 4) & 0x7;  // 8 строк, младшие 3 бита
}

int cache_get_tag(int address) {
    return address >> 7;  // Старшие биты
}

int cache_find_lru(void) {
    int lru_index = 0;
    int min_used = cache.lines[0].last_used;
    
    for (int i = 1; i < 8; i++) {
        if (cache.lines[i].last_used < min_used) {
            min_used = cache.lines[i].last_used;
            lru_index = i;
        }
    }
    return lru_index;
}

int cache_read(int address, int *value) {
    cache.access_count++;
    
    int index = cache_get_index(address);
    int tag = cache_get_tag(address);
    
    if (cache.lines[index].valid && cache.lines[index].tag == tag) {
        // Попадание в кэш
        cache.hits++;
        cache.lines[index].last_used = ++cache.lru_counter;
        *value = cache.lines[index].data;
        return 0;
    }
    
    // Промах - читаем из памяти
    cache.misses++;
    
    int mem_value;
    if (sc_memoryGet(address, &mem_value) == -1) {
        return -1;
    }
    
    // Загружаем в кэш
    cache.lines[index].valid = 1;
    cache.lines[index].tag = tag;
    cache.lines[index].data = mem_value;
    cache.lines[index].last_used = ++cache.lru_counter;
    
    *value = mem_value;
    return 0;
}

int cache_write(int address, int value) {
    cache.access_count++;
    
    int index = cache_get_index(address);
    int tag = cache_get_tag(address);
    
    // Записываем в память
    if (sc_memorySet(address, value) == -1) {
        return -1;
    }
    
    // Обновляем кэш если есть
    if (cache.lines[index].valid && cache.lines[index].tag == tag) {
        cache.lines[index].data = value;
        cache.lines[index].last_used = ++cache.lru_counter;
        cache.hits++;
    } else {
        cache.misses++;
        // Загружаем новую строку
        cache.lines[index].valid = 1;
        cache.lines[index].tag = tag;
        cache.lines[index].data = value;
        cache.lines[index].last_used = ++cache.lru_counter;
    }
    
    return 0;
}

int cache_get_hits(void) {
    return cache.hits;
}

int cache_get_misses(void) {
    return cache.misses;
}

void cache_print_stats(void) {
    printf("=== Статистика кэша ===\n");
    printf("Обращений: %d\n", cache.access_count);
    printf("Попаданий: %d (%.1f%%)\n", cache.hits, 
           cache.access_count ? (float)cache.hits / cache.access_count * 100 : 0);
    printf("Промахов: %d (%.1f%%)\n", cache.misses,
           cache.access_count ? (float)cache.misses / cache.access_count * 100 : 0);
}