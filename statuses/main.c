#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "statuses.h"

int init_db(char* filename);
int insert(Stat * obj, char* filename);
int delete(char* filename, Stat * d1);
int select(char* filename, int start, int end);
int proc_quer(char* command);
int solution(char* filename);
int proc_ins(char* command, Stat * obj);
int proc_sel(char* command, int * start, int * end);
int proc_upd(char* command, Stat * old, Stat * new);
int update(char* filename, Stat * old, Stat * new);


int main() {
    char filename[] = "statuses_db";

    if (!init_db(filename)) {
        printf("critical error");
    } else {
        solution(filename);
    }

    return 0;
}

int insert(Stat * obj, char* filename) {
    int err = 1;
    FILE * file = fopen(filename, "ab");
    if (!file) {
        err = 0;
    } else {
        err = fwrite(obj, sizeof(Stat), 1, file);
    }
    if (file) fclose(file);
    return err;
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
            Stat obj;
            resp = proc_ins(word, &obj);
            if (resp) {
                sucs = insert(&obj, filename);
                if (!sucs) printf("n/a\n");
            } else {
                printf("n/a\n");
            }
        } else if (ans == 1) {
            Stat old, new;
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
            Stat obj;
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

int init_db(char* filename) {
    int err = 1;
    FILE * file = fopen(filename, "ab");
    if (!file) {
        err = 0;
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

int proc_ins(char* command, Stat * obj) {  // insert  0 0 1 05.11.2020 08:43:59
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
            int b = (command[j] - '0') * pow(10, cnt);
            arr[k] += b;
            // printf("j = %d, a = %d   ", j , a);
            cnt--;
        }
        // printf("\nanswer = %d\n", arr[k]);
        k++;
    }
    char dt[11];
    char tm[9];
    k = 0;
    while (k < 2) {
        for (; i < n; i++) {
            if ((command[i] >= '0') && (command[i] <= '9'))
                break;
        }
        int len = 8;
        if (k == 0) len = 10;
        for (int j = i; j < len + i; j++) {
            if (k == 0)
                dt[j-i] = command[j];
            else
                tm[j-i] = command[j];
            // printf("%c", command[j]);
        }
        i += len;
        // printf("%c", command[i]);
        if ((command[i] != ' ') && (k == 0))
            return 0;
        k++;
    }
    dt[10] = '\0';
    tm[8] = '\0';
    obj -> ev = arr[0];
    obj -> mod = arr[1];
    obj -> stat = arr[2];
    strcpy(obj -> ch_dt, dt);
    strcpy(obj -> ch_tm, tm);
    return 1;
}

/*
 ак работает update?
Ќачинаем, как и обычно, с ключевого слова update
ѕотом через пробел или зап€тую вводим 6 чисел
первые 3 - это данные клетки, которую надо помен€ть,
последние 3 - то, на что надо помен€ть
*/
int update(char* filename, Stat * old, Stat * new) {
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
    int n = ftell(file)/sizeof(Stat);
    rewind(file);
    if (n == 0) {
        fclose(file);
        fclose(temp);
        return 0;
    }
    size_t sajz = sizeof(Stat);
    while (!feof(file)) {
        Stat d;
        int succ = fread(&d, sajz, 1, file);
        if (succ) {
            if (!((d.ev == old -> ev) && (d.mod == old -> mod) \
            && (d.stat == old -> stat) && (!strcmp(d.ch_dt, old -> ch_dt))\
            && (strcmp(d.ch_tm, old -> ch_tm) == 0))) {
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

int proc_upd(char* command, Stat * old, Stat * new) {
    int cnt = -1;
    int i = 6;
    int a = 0;

    int n = strlen(command);
    if (n < 58) return 0;  // хот€ кака€-то защита
    while (a < 2) {
        int arr[3]; arr[0] = 0; arr[1] = 0; arr[2] = 0;
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
                int b = (command[j] - '0') * pow(10, cnt);
                arr[k] += b;
                // printf("j = %d, a = %d   ", j , a);
                cnt--;
            }
            // printf("\nanswer = %d\n", arr[k]);
            k++;
        }
        char dt[11];
        char tm[9];
        k = 0;
        while (k < 2) {
            for (; i < n; i++) {
                if ((command[i] >= '0') && (command[i] <= '9'))
                    break;
            }
            int len = 8;
            if (k == 0) len = 10;
            for (int j = i; j < len + i; j++) {
                if (k == 0)
                    dt[j-i] = command[j];
                else
                    tm[j-i] = command[j];
                printf("%c", command[j]);
            }
            i += len;
            // printf("%c\n", command[i]);

            k++;
        }  // update 0 0 1 05.11.2020 08:43:59 0 0 1 05.11.2020 08:45:12
        dt[10] = '\0';
        tm[8] = '\0';
        if (a == 0) {
            old -> ev = arr[0];
            old -> mod = arr[1];
            old -> stat = arr[2];
            strcpy(old -> ch_dt, dt);
            strcpy(old -> ch_tm, tm);
        } else {
            new -> ev = arr[0];
            new -> mod = arr[1];
            new -> stat = arr[2];
            strcpy(new -> ch_dt, dt);
            strcpy(new -> ch_tm, tm);
        }
        a++;
    }

    return 1;
}

int delete(char* filename, Stat * d1) {
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
    int n = ftell(file)/sizeof(Stat);
    rewind(file);
    if (n == 0) {
        fclose(file);
        fclose(temp);
        return 0;
    }
    size_t sajz = sizeof(Stat);
    while (!feof(file)) {
        Stat d;
        int succ = fread(&d, sajz, 1, file);
        if (succ) {
            if (!((d.ev == d1 -> ev) && (d.mod == d1 -> mod) \
            && (d.stat == d1 -> stat) && (!strcmp(d.ch_dt, d1 -> ch_dt))\
            && (strcmp(d.ch_tm, d1 -> ch_tm) == 0))) {
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

int select(char* filename, int start, int end) {
    if (start > end)
        return 0;
    FILE * file = fopen(filename, "rb");
    if (!file) {
        return 0;
    }
    int i = 0;
    size_t sajz = sizeof(Stat);
    Stat obj;

    if ((end == 0) && (start == 0)) {
        fseek(file, 0, SEEK_END);
        int n = ftell(file)/sizeof(Stat);
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
            printf("%d %d %d %s %s\n", obj.ev, obj.mod, obj.stat, obj.ch_dt, obj.ch_tm);
            i++;
        }
    }
    fclose(file);
    return 1;
}
