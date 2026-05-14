#include <stdio.h>   // для printf
#include <stdlib.h>  // для malloc, free
#include <string.h>  // для strcpy, strlen

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Использование: %s <строка>\n", argv[0]);
        return 1;
    }

    // Выделяем память в куче — всего 8 байт
    char *buffer = (char *)malloc(8);

    // ДЕФЕКТ: копируем ввод пользователя без проверки размера
    // Если argv[1] больше 7 символов — выход за границу буфера
    strcpy(buffer, argv[1]);

    printf("Буфер: %s\n", buffer);

    // При освобождении повреждённой памяти — ошибка
    free(buffer);
    return 0;
}
