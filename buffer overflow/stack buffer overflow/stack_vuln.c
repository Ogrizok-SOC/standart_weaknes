#include <stdio.h>   // printf
#include <string.h>  // strcpy

// Функция-цель — её должен вызвать эксплойт
void secret() {
    printf("[!!!] ВРЕДОНОСНАЯ ФУНКЦИЯ ВЫПОЛНЕНА\n");
    printf("[!!!] Эксплойт успешен\n");
}

// Уязвимая функция
void vuln(char *input) {
    char buffer[16];  // буфер на стеке — 16 байт

    // ДЕФЕКТ: strcpy копирует без проверки размера
    // Если input > 15 символов — перезаписывается адрес возврата
    strcpy(buffer, input);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Использование: %s <строка>\n", argv[0]);
        printf("Адрес secret: %p\n", (void *)secret);
        return 1;
    }

    vuln(argv[1]);  // вызов уязвимой функции

    printf("Возврат из vuln.\n");  // при атаке сюда не попадём
    return 0;
}
