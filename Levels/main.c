#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "levels.h"


static int init_db(char* filename);
int insert(Lvl * obj, char* filename);
int delete(char* filename, Lvl * d1);
int select(char* filename, int start, int end);
int proc_quer(char* command);
int solution(char* filename);
int proc_ins(char* command, Lvl * obj);
int proc_sel(char* command, int * start, int * end);
int proc_upd(char* command, Lvl * old, Lvl * new);
int update(char* filename, Lvl * old, Lvl * new);


int main() {
    char filename[] = "levels_db";

    if (!init_db(filename)) {
        printf("critical error");
    } else {
        solution(filename);
    }

    return 0;
}

/*
Как пользоваться insert?
Введите insert (можно и InSert, и INSERt и т.д.)
затем 3 числа через пробел или запятую: 100 210 212
Итого: insert x y z; Где x, y, z - положительные целые числа
*/

int insert(Lvl * obj, char* filename) {
    int err = 1;
    FILE * file = fopen(filename, "ab");
    if (!file) {
        err = 0;
    } else {
        err = fwrite(obj, sizeof(Lvl), 1, file);
    }
    if (file) fclose(file);
    return err;
}

int proc_quer(char* command) {
    char option[][7] = {"insert", "update", "select", "delete", "exit"};
    int opt = -1;
    int i;
    if (!(strlen(command) < 6)) {
        int j = 0;
        while ((opt == -1) && (j < 4)) {
            for (i = 0; i < 6; i++) {
                if ((command[i] != option[j][i]) && (command[i] != option[j][i] - 32)) {
                    break;
                }
            }
            if (i == 6)
                opt = j;
            j++;
        }
    }
    if ((strlen(command) >= 4)) {
        for (i = 0; i < 4; i++) {
            if ((command[i] != option[4][i]) && (command[i] != option[4][i] - 32))
                break;
        }
        if (i == 4)
            opt = 4;
    }
    return opt;
}

int solution(char * filename) {
    int fl = 1;
    int resp;
    int sucs;
    while (fl) {
        char word[250];
        fgets(word, sizeof(word), stdin);
        word[strlen(word) - 1] = '\0';
        int ans = proc_quer(word);
        if (ans == 0) {
            Lvl obj;
            resp = proc_ins(word, &obj);
            if (resp) {
                sucs = insert(&obj, filename);
                if (!sucs) printf("n/a\n");
            } else {
                printf("n/a\n");
            }
        } else if (ans == 1) {
            Lvl old, new;
            resp = proc_upd(word, &old, &new);
            if (resp) {
                sucs = update(filename, &old, &new);
                if (!sucs) printf("n/a\n");
            } else {
                printf("n/a\n");
            }
        } else if (ans == 2) {
            int start; int end;
            resp = proc_sel(word, &start, &end);
            if (resp) {
                sucs = select(filename, start, end);
                if (!sucs) printf("n/a\n");
            } else {
                printf("n/a\n");
            }
        } else if (ans == 3) {
            Lvl obj;
            resp = proc_ins(word, &obj);
            if (resp) {
                sucs = delete(filename, &obj);
                if (!sucs) printf("n/a\n");
            } else {
                printf("n/a\n");
            }

        } else if (ans == 4) {
            fl = 0;
        } else {
            printf("n/a\n");
        }
    }
    return 1;
}

// Подходит как для обработки insert, так и delete
int proc_ins(char* command, Lvl * obj) {  // insert  100 200, 300
    int cnt = -1;
    int i = 6;
    int arr[3]; arr[0] = 0; arr[1] = 0; arr[2] = 0;
    int n = strlen(command);
    int k = 0;
    while (k < 3) {
        for (; i < n; i++) {
            if ((command[i] <= '9') && (command[i] >= '0'))
                break;
            else if ((command[i] != ' ') && (command[i] != ','))
                return 0;
            // printf("%d ", i);
        }  // insert 1 2 3
        // printf("\nprocceded into counting\n");
        for (; i < n; i++) {
            // printf("%d ", i);
            if ((command[i] <= '9') && (command[i] >= '0'))
                cnt++;
            else if ((command[i] == ',') || (command[i] == ' '))
                break;
            else
                return 0;
        }
        // printf("\ngot out of counting, cnt = %d\n", cnt);
        if (cnt == -1)
            return 0;
        for (int j = i - cnt - 1; j < i; j++) {
            int a = (command[j] - '0') * pow(10, cnt);
            arr[k] += a;
            // printf("j = %d, a = %d   ", j , a);
            cnt--;
        }
        // printf("\nanswer = %d\n", arr[k]);
        k++;
    }
    obj -> level = arr[0];
    obj -> cell = arr[1];
    obj -> flag = arr[2];

    return 1;
}

static  int init_db(char* filename) {
    int err = 1;
    FILE * file = fopen(filename, "ab");
    if (!file) {
        err = 0;
    }
    if (file) fclose(file);
    return err;
}

int proc_sel(char* command, int * start, int * end) {
    int i = 7;
    int cnt = -1;
    int arr[2]; arr[0] = 0; arr[1] = 0;
    int n = strlen(command);
    int k = 0;
    for (; i < n; i++) {
        // printf("%d ", i);
        if (command[i] == '*') {
            *end = 0;
            *start = 0;
            return 1;
        } else if ((command[i] <= '9') && (command[i] >= '0')) {
            continue;
        } else if (command[i] != ' ') {
            return 0;
        }
    }
    i = 7;
    // printf("%d\n", i);
    while (k < 2) {
        for (; i < n; i++) {
            if ((command[i] <= '9') && (command[i] >= '0'))
                break;
            else if ((command[i] != ' ') && (command[i] != ','))
                return 0;
            // printf("%d ", i);
        }
        // printf("\nprocceded into counting\n");
        for (; i < n; i++) {
            // printf("%d ", i);
            if ((command[i] <= '9') && (command[i] >= '0'))
                cnt++;
            else if ((command[i] == ',') || (command[i] == ' '))
                break;
            else
                return 0;
        }
        // printf("\ngot out of counting, cnt = %d\n", cnt);
        if (cnt == -1)
            return 0;
        for (int j = i - cnt - 1; j < i; j++) {
            int a = (command[j] - '0') * pow(10, cnt);
            arr[k] += a;
            // printf("j = %d, a = %d   ", j , a);
            cnt--;
        }
        // printf("\nanswer = %d\n", arr[k]);
        k++;
    }
    *start = arr[0];
    *end = arr[1];

    return 1;
}

/*
как пользоваться select?
Введите ключевое слово select,
затем 2 целочисленных положительных числа через пробел или запятую
Итого: select x, y;
Где x - начало, а y - конец.
Так же можно ввести select *
Тогда выведется всё содержимое файла
Точно так же работает select 0 0
*/
int select(char* filename, int start, int end) {
    if (start > end)
        return 0;
    FILE * file = fopen(filename, "rb");
    if (!file) {
        return 0;
    }
    int i = 0;
    size_t sajz = sizeof(Lvl);
    Lvl obj;

    if ((end == 0) && (start == 0)) {
        fseek(file, 0, SEEK_END);
        int n = ftell(file)/sizeof(Lvl);
        rewind(file);
        end = n;
    }
    while ((i < start) && (!feof(file))) {
        fread(&obj, sajz, 1, file);
        i++;
    }
    while ((i <= end) && (!feof(file))) {
        int succ = fread(&obj, sajz, 1, file);
        if (succ) {
            printf("%d %d %d\n", obj.level, obj.cell, obj.flag);
            i++;
        }
    }
    fclose(file);
    return 1;
}


/*
Как пользоваться delete?
Сначала вводим ключевое delete
Затем элементы строчки, которые надо удалить
Т.е. delete x, y, z (можно и пробелы, и запятые)
Дело в том, что я словил неопределённость с тем, какие фильтры
могут быть для удаления всего, что между, поэтому удаление идёт по экземпляру
*/

int delete(char* filename, Lvl * d1) {
    char help[100];
    strcpy(help, filename);
    strcat(help, "__temp");
    FILE * file = fopen(filename, "rb");
    FILE * temp = fopen(help, "wb");
    if ((!file) || (!temp)) {
        if (file) fclose(file);
        if (temp) fclose(temp);
        return 0;
    }
    fseek(file, 0, SEEK_END);
    int n = ftell(file)/sizeof(Lvl);
    rewind(file);
    if (n == 0) {
        fclose(file);
        fclose(temp);
        return 0;
    }
    size_t sajz = sizeof(Lvl);
    while (!feof(file)) {
        Lvl d;
        int succ = fread(&d, sajz, 1, file);
        if (succ) {
            if (!((d.flag == d1 -> flag) && (d.level == d1 -> level) \
            && (d.cell == d1 -> cell))) {
                fwrite(&d, sajz, 1, temp);
            }
        }
    }
    fclose(file);
    fclose(temp);
    remove(filename);
    rename(help, filename);
    return 1;
}

int proc_upd(char* command, Lvl * old, Lvl * new) {
    int cnt = -1;
    int i = 6;
    int arr[6];
    for (int l = 0; l < 6; l++) arr[l] = 0;
    int n = strlen(command);
    int k = 0;
    while (k < 6) {
        for (; i < n; i++) {
            if ((command[i] <= '9') && (command[i] >= '0'))
                break;
            else if ((command[i] != ' ') && (command[i] != ','))
                return 0;
            // printf("%d ", i);
        }  // insert 1 2 3
        // printf("\nprocceded into counting\n");
        for (; i < n; i++) {
            // printf("%d ", i);
            if ((command[i] <= '9') && (command[i] >= '0'))
                cnt++;
            else if ((command[i] == ',') || (command[i] == ' '))
                break;
            else
                return 0;
        }
        // printf("\ngot out of counting, cnt = %d\n", cnt);
        if (cnt == -1)
            return 0;
        for (int j = i - cnt - 1; j < i; j++) {
            int a = (command[j] - '0') * pow(10, cnt);
            arr[k] += a;
            // printf("j = %d, a = %d   ", j , a);
            cnt--;
        }
        // printf("\nanswer = %d\n", arr[k]);
        k++;
    }
    old -> level = arr[0];
    old -> cell = arr[1];
    old -> flag = arr[2];
    new -> level = arr[3];
    new -> cell = arr[4];
    new -> flag = arr[5];

    return 1;
}

/*
Как работает update?
Начинаем, как и обычно, с ключевого слова update
Потом через пробел или запятую вводим 6 чисел
первые 3 - это данные клетки, которую надо поменять,
последние 3 - то, на что надо поменять
*/
int update(char* filename, Lvl * old, Lvl * new) {
    char help[100];
    strcpy(help, filename);
    strcat(help, "__temp");
    FILE * file = fopen(filename, "rb");
    FILE * temp = fopen(help, "wb");
    if ((!file) || (!temp)) {
        if (file) fclose(file);
        if (temp) fclose(temp);
        return 0;
    }
    fseek(file, 0, SEEK_END);
    int n = ftell(file)/sizeof(Lvl);
    rewind(file);
    if (n == 0) {
        fclose(file);
        fclose(temp);
        return 0;
    }
    size_t sajz = sizeof(Lvl);
    while (!feof(file)) {
        Lvl d;
        int succ = fread(&d, sajz, 1, file);
        if (succ) {
            if (!((d.flag == old -> flag) && (d.level == old -> level) \
            && (d.cell == old -> cell))) {
                fwrite(&d, sajz, 1, temp);
            } else {
                fwrite(new, sajz, 1, temp);
            }
        }
    }
    fclose(file);
    fclose(temp);
    remove(filename);
    rename(help, filename);
    return 1;
}
