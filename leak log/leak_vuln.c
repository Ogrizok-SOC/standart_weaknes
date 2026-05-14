#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Использование: %s <логин> <пароль>\n", argv[0]);
        return 1;
    }

    char *login = argv[1];
    char *password = argv[2];
    char *correct_password = "Secret123";

    printf("[LOG] Попытка входа: логин=%s\n", login);

    // УЯЗВИМОСТЬ: пароль попадает в лог ошибки
    if (strcmp(password, correct_password) != 0) {
        printf("[ERROR] Неверный пароль для %s. Введено: %s, ожидалось: %s\n",
               login, password, correct_password);
        return 1;
    }

    printf("[OK] Вход выполнен\n");
    return 0;
}
