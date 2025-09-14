// #include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 80
#define HEIGHT 25
// #define SPEED_MIN 10
// #define SPEED_MAX 100
/*  Меню выбора файла  */
int file_selection() {
    int selected;

    printf("Выберите паттерн:\n");
    printf("1 - Blinker (blinker.txt)\n");
    printf("2 - Eater (eater.txt)\n");
    printf("3 - Glider (glider.txt)\n");

    scanf("%d", &selected);
    return selected;
}
/*  Загрузка файла в двухмерный массив  */
int load_file(int field[HEIGHT][WIDTH], int selected) {
    FILE *file = NULL;
    if (selected == 1) {
        file = fopen("blinker.txt", "r");
    } else if (selected == 2) {
        file = fopen("eater.txt", "r");
    } else {
        printf("n/a");
    }

    if (file != NULL) {
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                int c = fgetc(file);
                if (c == '1') {
                    field[y][x] = 1;
                } else if (c == '0') {
                    field[y][x] = 0;
                }
            }
            // Пропускаем символ новой строки (если есть)
            int newline = fgetc(file);
            if (newline != '\n' && newline != EOF) {
                fclose(file);
                return 1;
            }
        }
        fclose(file);
    } else {
        printf("Ошибка: Проблема с файлом\n");
        return 1;
    }

    return 0;
}
/*  Отрисовка игрового поля  */
void draw_field(int field[HEIGHT][WIDTH]) {
    printf("\033[2J\033[H");  // Очистка экрана

    // Заполнение строк и столбцов
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (field[y][x] == 1) {
                printf("O");  // Живая клетка
            } else {
                printf(".");  // Мёртвая клетка
            }
        }
        printf("\n");
    }
}

char get_command(void) {
    char cmd;
    printf("Press 'n' for next generation, ' ' to quit: ");
    fflush(stdout);

    while (1) {
        cmd = getchar();
        if (cmd == 'n') {
            return 'n';
        } else if (cmd == ' ') {
            return ' ';
        }
        // Игнорируем любые другие символы
    }
}

void evolve_field(int field[HEIGHT][WIDTH], int next[HEIGHT][WIDTH]) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int neighbors = 0;

            // Перебираем 8 соседей (включая диагонали)
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    if (dy == 0 && dx == 0) continue;  // пропускаем саму клетку

                    // Тороидальная геометрия: края соединяются
                    int ny = (y + dy + HEIGHT) % HEIGHT;
                    int nx = (x + dx + WIDTH) % WIDTH;

                    neighbors += field[ny][nx];
                }
            }

            // Правила Конвея
            if (field[y][x] == 1) {
                // Живая клетка
                next[y][x] = (neighbors == 2 || neighbors == 3) ? 1 : 0;
            } else {
                // Мёртвая клетка
                next[y][x] = (neighbors == 3) ? 1 : 0;
            }
        }
    }
}

void copy_field(int field[HEIGHT][WIDTH], int next[HEIGHT][WIDTH]) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            field[y][x] = next[y][x];
        }
    }
}

int main(void) {
    int field[HEIGHT][WIDTH] = {0};
    int next[HEIGHT][WIDTH] = {0};

    int pattern = file_selection();
    load_file(field, pattern);

    // draw_field(field);

    while (1) {
        char cmd = get_command();

        if (cmd == ' ') {
            break;
        } else if (cmd == 'n') {
            evolve_field(field, next);
            copy_field(field, next);
            draw_field(field);
        }
    }
    return 0;
}
