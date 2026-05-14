#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Безопасное умножение с проверкой переполнения
int safe_multiply(uint32_t a, uint32_t b, size_t *result) {
    uint64_t tmp = (uint64_t)a * (uint64_t)b;

    // Проверяем, помещается ли в size_t
    if (tmp > SIZE_MAX) {
        return 0;  // Переполнение
    }

    *result = (size_t)tmp;
    return 1;  // Успех
}

int main() {
    printf("=== Безопасная программа (исправленная) ===\n\n");

    printf("1. Беззнаковое переполнение (unsigned short):\n");
    unsigned short a = 65535;
    // ИСПРАВЛЕНИЕ: проверка перед сложением
    if (a < 65535) {
        unsigned short b = a + 1;
        printf("   65535 + 1 = %u\n", b);
    } else {
        printf("   [!] Переполнение! %u + 1 не влезает в unsigned short\n", a);
    }

    printf("\n2. Знаковое переполнение (int):\n");
    int max_int = 2147483647;
    // ИСПРАВЛЕНИЕ: проверка перед сложением
    if (max_int < 2147483647) {
        int overflowed = max_int + 1;
        printf("   2147483647 + 1 = %d\n", overflowed);
    } else {
        printf("   [!] Переполнение! %d + 1 не влезает в int\n", max_int);
    }

    printf("\n3. Безопасное выделение памяти:\n");

    unsigned int count;
    printf("   Введите количество элементов: ");
    scanf("%u", &count);

    if (count == 0) {
        printf("   [!] Некорректное количество\n");
        return 1;
    }

    // ИСПРАВЛЕНИЕ: проверка переполнения перед выделением
    size_t size;
    if (!safe_multiply(count, sizeof(int), &size)) {
        printf("   [!] Ошибка: переполнение при вычислении размера!\n");
        printf("   [!] %u * %zu = слишком много\n", count, sizeof(int));
        return 1;
    }

    printf("   Запрошено: %u элементов\n", count);
    printf("   Безопасный размер: %zu байт\n", size);

    int *buffer = (int *)malloc(size);
    if (buffer == NULL) {
        printf("   [!] Ошибка выделения памяти\n");
        return 1;
    }

    printf("   [*] Память выделена корректно\n");

    // Запись данных (без переполнения)
    for (unsigned int i = 0; i < count; i++) {
        buffer[i] = i;
    }

    printf("   [*] Данные записаны\n");
    free(buffer);
    printf("   [*] Готово\n");

    return 0;
}
