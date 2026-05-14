#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int main() {
    printf("=== Демонстрация Integer Overflow ===\n\n");

    // 1. Беззнаковое переполнение (wraparound)
    printf("1. Беззнаковое переполнение (unsigned short):\n");
    unsigned short a = 65535;
    unsigned short b = a + 1;
    printf("   65535 + 1 = %u (wraparound!)\n\n", b);

    // 2. Знаковое переполнение
    printf("2. Знаковое переполнение (int):\n");
    int max_int = 2147483647;
    int overflowed = max_int + 1;
    printf("   2147483647 + 1 = %d (UB!)\n\n", overflowed);

    // 3. УЯЗВИМОСТЬ: 32-битное умножение перед присваиванием 64-битному типу
    printf("3. Уязвимость выделения памяти:\n");
    printf("   (имитация 32-битной системы через uint32_t)\n");

    unsigned int count;
    printf("   Введите количество элементов: ");
    scanf("%u", &count);

    if (count == 0) {
        printf("   [!] Некорректное количество\n");
        return 1;
    }

    // УЯЗВИМОСТЬ: умножение в 32-битном типе
    uint32_t size32 = count * sizeof(int);      // Переполнение здесь!
    size_t size = (size_t)size32;                // Расширение до 64 бит (уже поздно)

    printf("   Запрошено: %u элементов\n", count);
    printf("   count * sizeof(int) в uint32_t: %u байт\n", size32);
    printf("   После приведения к size_t: %zu байт\n", size);

    if (size32 == 0 && count > 0) {
        printf("   [!] ПЕРЕПОЛНОНИЕ! Размер стал 0!\n");
    }

    int *buffer = (int *)malloc(size);
    if (buffer == NULL) {
        printf("   [!] malloc вернул NULL (size=%zu)\n", size);
        return 1;
    }

    printf("   [*] Память выделена\n");

    // Запись в "маленький" буфер с большим количеством итераций
    for (unsigned int i = 0; i < count; i++) {
        buffer[i] = i;  // Heap overflow!
    }

    printf("   [*] Данные записаны\n");
    free(buffer);
    return 0;
}
