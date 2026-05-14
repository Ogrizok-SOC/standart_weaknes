#include <stdio.h>   // printf
#include <string.h>  // strncpy

// Функция осталась для демонстрации, что её нельзя вызвать
void secret() {
    printf("[!!!] ВРЕДОНОСНАЯ ФУНКЦИЯ\n");
}

// Исправленная функция
void vuln(char *input) {
    char buffer[16];  // буфер на стеке — 16 байт

    // ИСПРАВЛЕНИЕ: strncpy копирует НЕ БОЛЬШЕ указанного размера
    // В отличие от strcpy, которая копирует всё без ограничений
    strncpy(buffer, input, 15);
    buffer[15] = '\0';  // гарантируем завершающий ноль
}

int main(int argc, char *argv[]) {
    if (argc < 2) return 1;

    vuln(argv[1]);

    printf("Возврат из vuln.\n");  // сюда мы попадаем всегда
    return 0;
}
