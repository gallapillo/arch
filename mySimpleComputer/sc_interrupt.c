#include "../include/simpleComputer.h"
#include <stdio.h>

static int interrupt_enabled = 1;
static int pending_interrupts[16] = {0};  // 16 векторов прерываний
static int interrupt_handler_addr = 0x00; // Адрес обработчика прерываний

int interrupt_init(void) {
    interrupt_enabled = 1;
    for (int i = 0; i < 16; i++) {
        pending_interrupts[i] = 0;
    }
    interrupt_handler_addr = 0x00;
    return 0;
}

int interrupt_enable(int enable) {
    interrupt_enabled = enable;
    return 0;
}

int interrupt_request(int vector) {
    if (vector < 0 || vector >= 16) {
        return -1;
    }
    pending_interrupts[vector] = 1;
    return 0;
}

int interrupt_handle(void) {
    if (!interrupt_enabled) {
        return -1;
    }
    
    // Проверяем наличие ожидающих прерываний
    for (int i = 0; i < 16; i++) {
        if (pending_interrupts[i]) {
            // Сохраняем текущий счетчик команд
            int current_ic;
            sc_icounterGet(&current_ic);
            
            // Устанавливаем обработчик
            sc_icounterSet(interrupt_handler_addr + i);
            
            // Сбрасываем флаг прерывания
            pending_interrupts[i] = 0;
            
            return i;  // Возвращаем номер обработанного прерывания
        }
    }
    
    return -1;
}

// Функция для установки адреса обработчика прерываний
void interrupt_set_handler(int addr) {
    interrupt_handler_addr = addr;
}

// Функция для проверки ожидающих прерываний
int interrupt_pending(void) {
    for (int i = 0; i < 16; i++) {
        if (pending_interrupts[i]) return 1;
    }
    return 0;
}