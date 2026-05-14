#include <stdio.h>
#include <string.h>

void secret_function() {
    printf("[!!!] СЕКРЕТНАЯ ФУНКЦИЯ ВЫПОЛНЕНА!\n");
}

void vulnerable_log(char *input) {
    char buffer[64];

    // Безопасное копирование в буфер
    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    // УЯЗВИМОСТЬ: пользовательский ввод передаётся напрямую как форматная строка
    printf("[LOG] ");
    printf(buffer);  // <-- ДЕФЕКТ: buffer используется как форматная строка
    printf("\n");

    // Правильный вариант: printf("[LOG] %s\n", buffer);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Использование: %s <сообщение>\n", argv[0]);
        printf("Пример: %s Hello\n", argv[0]);
        return 1;
    }

    printf("[*] Адрес secret_function: %p\n", (void *)secret_function);
    printf("[*] Вызов vulnerable_log...\n");
    vulnerable_log(argv[1]);
    printf("[*] Готово\n");

    return 0;
}
