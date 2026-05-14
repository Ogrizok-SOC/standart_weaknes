#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Использование: %s <строка>\n", argv[0]);
        return 1;
    }

    char *buffer = (char *)malloc(8);

    // ИСПРАВЛЕНИЕ: strncpy не выходит за границу буфера
    strncpy(buffer, argv[1], 7);  // максимум 7 символов
    buffer[7] = '\0';             // гарантируем завершающий ноль

    printf("Буфер: %s\n", buffer);
    free(buffer);
    return 0;
}
