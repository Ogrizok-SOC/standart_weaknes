#include <stdio.h>
#include <string.h>

void secret_function() {
    printf("[!!!] СЕКРЕТНАЯ ФУНКЦИЯ ВЫПОЛНЕНА!\n");
}

void safe_log(char *input) {
    char buffer[64];

    // Безопасное копирование в буфер
    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    // ИСПРАВЛЕНИЕ: фиксированная форматная строка, buffer как аргумент %s
    printf("[LOG] %s\n", buffer);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Использование: %s <сообщение>\n", argv[0]);
        return 1;
    }

    printf("[*] Вызов safe_log...\n");
    safe_log(argv[1]);
    printf("[*] Готово\n");

    return 0;
}
