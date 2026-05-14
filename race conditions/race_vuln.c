#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Использование: %s <файл> <данные>\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];
    char *data = argv[2];
    struct stat st;
    int fd;

    // Проверяем, что файл обычный
    if (stat(filename, &st) != 0) {
        printf("[!] Файл не найден\n");
        return 1;
    }

    if (!S_ISREG(st.st_mode)) {
        printf("[!] Это не обычный файл — отказ\n");
        return 1;
    }

    printf("[*] Файл обычный, проверка пройдена\n");
    printf("[*] Задержка 5 секунд (жертва \"думает\")...\n");
    printf("[*] Быстро сделай атаку в другом терминале!\n");
    
    // УЯЗВИМОСТЬ: задержка 5 секунд
    sleep(10);

    fd = open(filename, O_WRONLY | O_APPEND);
    if (fd < 0) {
        printf("[!] Ошибка открытия\n");
        return 1;
    }

    printf("[*] Запись данных: %s\n", data);
    write(fd, data, strlen(data));
    write(fd, "\n", 1);
    close(fd);
    printf("[*] Готово\n");

    return 0;
}
