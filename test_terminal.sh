#!/bin/bash

# test_terminal.sh - Скрипт для проверки escape-последовательностей терминала

# Определяем escape-последовательности через infocmp
CLEAR=$(infocmp -1 | grep -o 'clear=\\E\[[0-9;]*H\\E\[J' | sed 's/clear=//')
CUP=$(infocmp -1 | grep -o 'cup=\\E\[[0-9;]*%p1%d;%p2%dH' | sed 's/cup=//')
SETAF=$(infocmp -1 | grep -o 'setaf=\\E\[[0-9;]*%p1%dm' | sed 's/setaf=//')
SETAB=$(infocmp -1 | grep -o 'setab=\\E\[[0-9;]*%p1%dm' | sed 's/setab=//')
CIVIS=$(infocmp -1 | grep -o 'civis=\\E\[?25l' | sed 's/civis=//')
CNORM=$(infocmp -1 | grep -o 'cnorm=\\E\[?12l\\E\[?25h' | sed 's/cnorm=//')
EL=$(infocmp -1 | grep -o 'el=\\E\[K' | sed 's/el=//')

echo "=== Проверка escape-последовательностей ==="

# Очистка экрана
echo -e "\033[H\033[J"

# Имя красными буквами на черном фоне (5 строка, 10 символ)
echo -e "\033[5;10H\033[31m\033[40mИванов Иван Иванович\033[0m"

# Группа зеленым на белом фоне (6 строка, 8 символ)
echo -e "\033[6;8H\033[32m\033[47mГруппа: ИУ8-xx\033[0m"

# Курсор в 10 строку, 1 символ
echo -e "\033[10;1H"

echo ""
echo "=== Проверка завершена ==="