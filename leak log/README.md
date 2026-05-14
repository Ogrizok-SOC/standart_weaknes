.
├── buffer overflow/
│ ├── heap buffer overflow/
│ │ ├── heap_vuln.c # уязвимая программа
│ │ ├── heap_fixed.c # исправленная программа
│ │ └── exploit.py # эксплойт
│ └── stack buffer overflow/
│ ├── stack_vuln.c # уязвимая программа
│ ├── stack_fixed.c # исправленная программа
│ └── exploit_stack.py # эксплойт
├── tainted input vulnerability/
│ ├── tainted_vuln.c # уязвимая программа
│ ├── tainted_fixed.c # исправленная программа
│ └── exploit.py # эксплойт
├── format string vulnerabilility/
│ ├── format_vuln.c # уязвимая программа
│ ├── format_fixed.c # исправленная программа
│ └── exploit_format.py # эксплойт
├── race conditions/
│ ├── race_vuln.c # уязвимая программа
│ ├── race_fixed.c # исправленная программа
│ └── exploit_race.sh # эксплойт
├── integer overflow/
│ ├── int_overflow.c # уязвимая программа
│ ├── int_fixed.c # исправленная программа
│ └── exploit_int.sh # эксплойт
└── leak log/
├── leak_vuln.c # уязвимая программа
├── leak_fixed.c # исправленная программа
└── exploit_leak.sh # эксплойт

text

---

## 1.1 Heap Buffer Overflow (CWE-122)

### Описание
Переполнение буфера, выделенного в динамической памяти (куче). При копировании данных без проверки размера происходит запись за границы буфера и повреждение служебных структур менеджера памяти.

### Уязвимость
```c
char *buffer = (char *)malloc(8);
strcpy(buffer, argv[1]);  // ДЕФЕКТ: нет проверки размера
Эксплуатация
bash
gcc -o heap_vuln heap_vuln.c
python3 exploit.py
Результат:

text
malloc(): corrupted top size
Aborted (core dumped)
Исправление
c
strncpy(buffer, argv[1], 7);
buffer[7] = '\0';
1.2 Stack Buffer Overflow (CWE-121)
Описание
Переполнение локального буфера на стеке. Позволяет перезаписать адрес возврата из функции и передать управление на вредоносный код.

Уязвимость
c
char buffer[16];
strcpy(buffer, input);  // ДЕФЕКТ: нет проверки размера
Эксплуатация
bash
gcc -fno-stack-protector -z execstack -no-pie -o stack_vuln stack_vuln.c
python3 exploit_stack.py
Результат:

text
[!!!] ВРЕДОНОСНАЯ ФУНКЦИЯ ВЫПОЛНЕНА
[!!!] Эксплойт успешен
Исправление
c
strncpy(buffer, input, 15);
buffer[15] = '\0';
2. Tainted Input / Command Injection (CWE-20 / CWE-77)
Описание
Пользовательский ввод передаётся в системную команду без проверки. Злоумышленник может выполнить произвольные команды через спецсимволы (;, &&, |).

Уязвимость
c
snprintf(command, sizeof(command), "ping -c 1 %s", host);
system(command);  // ДЕФЕКТ: нет проверки host
Эксплуатация
bash
gcc -o tainted_vuln tainted_vuln.c
./tainted_vuln "google.com; whoami"
Результат:

text
PING google.com ...
nikitos
Команда whoami выполнена.

Исправление
c
int is_valid_host(char *host) {
    for (int i = 0; host[i] != '\0'; i++) {
        if (!isalnum(host[i]) && host[i] != '.' && host[i] != '-')
            return 0;
    }
    return 1;
}
3. Format String Vulnerability (CWE-134)
Описание
Пользовательский ввод передаётся напрямую как форматная строка в printf. Злоумышленник может читать память стека (%x) или записывать данные (%n).

Уязвимость
c
printf(buffer);  // ДЕФЕКТ: buffer — форматная строка
Эксплуатация
bash
gcc -o format_vuln format_vuln.c
./format_vuln "%x %x %x %x %x"
Результат:

text
[LOG] 474f4c5b 0 0 61803b20 410
Содержимое стека выведено.

Исправление
c
printf("%s", buffer);
// или
printf("[LOG] %s\n", buffer);
4. Race Condition / TOCTOU (CWE-367)
Описание
Между проверкой файла (stat) и его использованием (open) проходит время. Злоумышленник может удалить файл и создать симлинк на другой файл. Программа проверит одно, а запишет в другое.

Уязвимость
c
stat(filename, &st);     // ПРОВЕРКА
sleep(10);               // окно для атаки
open(filename, ...);     // ИСПОЛЬЗОВАНИЕ — файл уже подменён
Эксплуатация
Терминал 1:

bash
echo "original data" > /tmp/original.txt
echo "ВАЖНЫЕ ДАННЫЕ" > /tmp/important.txt
./race_vuln /tmp/original.txt "ЗЛОВРЕДНЫЕ ДАННЫЕ"
Терминал 2 (пока жертва спит):

bash
rm /tmp/original.txt
ln -s /tmp/important.txt /tmp/original.txt
Результат:

bash
cat /tmp/important.txt
# ВАЖНЫЕ ДАННЫЕ
# ЗЛОВРЕДНЫЕ ДАННЫЕ   ← данные записались не туда
Исправление
c
fd = open(filename, O_WRONLY | O_APPEND | O_NOFOLLOW);
fstat(fd, &st);  // проверка по дескриптору, не по имени
5. Integer Overflow (CWE-190)
Описание
Результат арифметической операции не помещается в отведённый тип данных. При вычислении размера буфера это приводит к выделению памяти меньше ожидаемого и последующему переполнению.

Уязвимость
c
uint32_t size32 = count * sizeof(int);  // переполнение при count=1073741824
int *buffer = (int *)malloc(size32);    // size32 = 0 при переполнении
Эксплуатация
bash
gcc -o int_overflow int_overflow.c
echo "1073741824" | ./int_overflow
Результат:

text
count * sizeof(int) в uint32_t: 0 байт
[!] ПЕРЕПОЛНОНИЕ! Размер стал 0!
Segmentation fault (core dumped)
Исправление
c
int safe_multiply(uint32_t a, uint32_t b, size_t *result) {
    uint64_t tmp = (uint64_t)a * (uint64_t)b;
    if (tmp > SIZE_MAX) return 0;  // переполнение
    *result = (size_t)tmp;
    return 1;
}
6. Information Leak via Error Logging (CWE-532)
Описание
Конфиденциальные данные (пароли, ключи) попадают в сообщения об ошибках или логи. Злоумышленник, имеющий доступ к логам, может извлечь эту информацию.

Уязвимость
c
printf("[ERROR] Неверный пароль. Введено: %s, ожидалось: %s\n",
       password, correct_password);  // ДЕФЕКТ: пароли в логе
Эксплуатация
bash
gcc -o leak_vuln leak_vuln.c
./leak_vuln admin 12345 2>&1 | tee /tmp/leaked.log
grep "ожидалось" /tmp/leaked.log
Результат:

text
[ERROR] Неверный пароль для admin. Введено: 12345, ожидалось: Secret123
Правильный пароль Secret123 утёк в лог.

Исправление
c
printf("[ERROR] Неверный пароль для пользователя %s\n", login);
Статический анализ
Все программы проверены с помощью:

cppcheck --enable=all

gcc -Wall -Wextra

gcc -fanalyzer

Уязвимость	Обнаружена стат. анализом?
Heap Buffer Overflow	Нет
Stack Buffer Overflow	Нет
Tainted Input	Нет
Format String	Да (-Wformat-security)
Race Condition	Нет
Integer Overflow	Нет
Information Leak	Нет
Примечание: согласно заданию, дефект может быть не обнаружен статическим анализатором — это нормально.

