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

    // ИСПРАВЛЕНИЕ: сначала открываем (O_NOFOLLOW блокирует симлинки)
    fd = open(filename, O_WRONLY | O_APPEND | O_NOFOLLOW);
    if (fd < 0) {
        printf("[!] Ошибка открытия (возможно, симлинк — отказ)\n");
        return 1;
    }

    // Проверяем УЖЕ ОТКРЫТЫЙ файл через fstat (не stat по имени!)
    if (fstat(fd, &st) != 0) {
        printf("[!] Ошибка fstat\n");
        close(fd);
        return 1;
    }

    if (!S_ISREG(st.st_mode)) {
        printf("[!] Не обычный файл — отказ\n");
        close(fd);
        return 1;
    }

    // Никаких задержек — пишем сразу в открытый дескриптор
    printf("[*] Проверка пройдена, запись...\n");
    write(fd, data, strlen(data));
    write(fd, "\n", 1);
    close(fd);
    printf("[*] Готово\n");

    return 0;
}
