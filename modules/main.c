#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "modules.h"


int init_db(char* filename);
int insert(Module * obj, char* filename);
int delete(char* filename, Module * d1);
int select(char* filename, int start, int end);
int proc_quer(char* command);
int solution(char* filename);
int proc_ins(char* command, Module * obj);
int proc_sel(char* command, int * start, int * end);
int proc_upd(char* command, Module * old, Module * new);
int update(char* filename, Module * old, Module * new);


int main() {
    char filename[] = "modules_db";

    if (!init_db(filename)) {
        printf("critical error");
    } else {
        solution(filename);
    }

    return 0;
}


int insert(Module * obj, char* filename) {
    int err = 1;
    FILE * file = fopen(filename, "ab");
    if (!file) {
        err = 0;
    } else {
        err = fwrite(obj, sizeof(Module), 1, file);
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
            Module obj;
            resp = proc_ins(word, &obj);
            if (resp) {
                sucs = insert(&obj, filename);
                if (!sucs) printf("n/a\n");
            } else {
                printf("n/a\n");
            }
        } else if (ans == 1) {
            Module old, new;
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
            Module obj;
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

int proc_ins(char* command, Module * obj) {  // insert
    int cnt = -1;
    int i = 6;
    int arr[3]; arr[0] = 0; arr[1] = 0; arr[2] = 0;
    int n = strlen(command);
    int k = 0;
    while (k < 1) {  // insert 7 Simple logic module 2 2 0
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
            // printf("j = %d, a = %d   ", j, a);
            cnt--;
        }
        // printf("\nanswer = %d\n", arr[k]);
        k++;
    }
    k = 0;

    char name[31];
    int counter = 0;
    for (; i < n; i++) {
        if ((command[i] >= 'a') && (command[i] <= 'z'))
            break;
        else if ((command[i] >= 'A') && (command[i] <= 'Z'))
            break;
        else if (command[i] == ' ')
            continue;
        else
            return 0;
    }
    if (i == n - 1) return 0;
    for (int j = i; j < n; j++) {
        if ((command[j] >= '0') && (command[j] <= '9'))
            break;
        name[counter] = command[i + counter];
        counter++;
        if (counter == 30) return 0;
    }
    i += counter;
    obj -> id = arr[0]; arr[0] = 0;
    name[counter - 1] = '\0';
    k = 0;
    while (k < 3) {  // 7 Simple logic module 2 2 0
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
    strcpy(obj -> name, name);
    obj -> loc = arr[0];
    obj -> cell_loc = arr[1];
    obj -> del = arr[2];
    // printf("%d %s %d %d %d\n", obj->id, obj->name, obj->loc, obj->cell_loc, obj->del);
    return 1;
}

/*
Как работает update?
Начинаем, как и обычно, с ключевого слова update
Потом через пробел или запятую вводим 6 чисел
первые 3 - это данные клетки, которую надо поменять,
последние 3 - то, на что надо поменять
*/
int update(char* filename, Module * old, Module * new) {
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
    int n = ftell(file)/sizeof(Module);
    rewind(file);
    if (n == 0) {
        fclose(file);
        fclose(temp);
        return 0;
    }
    size_t sajz = sizeof(Module);
    while (!feof(file)) {
        Module d;
        int succ = fread(&d, sajz, 1, file);
        if (succ) {
            if (!((d.id == old -> id) && (d.loc == old -> loc) \
            && (d.cell_loc == old -> cell_loc) && ((d.del == old -> del))\
            && (strcmp(d.name, old -> name) == 0))) {
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

int proc_upd(char* command, Module * old, Module * new) {
    int cnt = -1;
    int i = 6;
    int a = 0;
    int n = strlen(command);
    while (a < 2) {
    int arr[3]; arr[0] = 0; arr[1] = 0; arr[2] = 0;

    int k = 0;
    while (k < 1) {  // insert 7 Simple logic module 2 2 0
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
            int b = (command[j] - '0') * pow(10, cnt);
            arr[k] += b;
            // printf("j = %d, a = %d   ", j, a);
            cnt--;
        }
        // printf("\nanswer = %d\n", arr[k]);
        k++;
    }
    k = 0;

    char name[31];
    int counter = 0;
    for (; i < n; i++) {
        if ((command[i] >= 'a') && (command[i] <= 'z'))
            break;
        else if ((command[i] >= 'A') && (command[i] <= 'Z'))
            break;
        else if (command[i] == ' ')
            continue;
        else
            return 0;
    }
    if (i == n - 1) return 0;
    for (int j = i; j < n; j++) {
        if ((command[j] >= '0') && (command[j] <= '9'))
            break;
        name[counter] = command[i + counter];
        counter++;
        if (counter == 30) return 0;
    }
    i += counter;
    if (a == 0) {
        new -> id = arr[0]; arr[0] = 0;
    } else {
        new -> id = arr[0]; arr[0] = 0;
    }
    name[counter - 1] = '\0';
    k = 0;
    while (k < 3) {  // 7 Simple logic module 2 2 0
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
            int d = (command[j] - '0') * pow(10, cnt);
            arr[k] += d;
            // printf("j = %d, a = %d   ", j , a);
            cnt--;
        }
        // printf("\nanswer = %d\n", arr[k]);
        k++;
    }
    if (a == 0) {
    strcpy(old -> name, name);
    old -> loc = arr[0];
    old -> cell_loc = arr[1];
    old -> del = arr[2];
    } else {
        strcpy(new -> name, name);
        new -> loc = arr[0];
        new -> cell_loc = arr[1];
        new -> del = arr[2];
    }
    a++;
    // printf("%d %s %d %d %d\n", obj->id, obj->name, obj->loc, obj->cell_loc, obj->del);
    }
    return 1;
}

int delete(char* filename, Module * d1) {
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
    int n = ftell(file)/sizeof(Module);
    rewind(file);
    if (n == 0) {
        fclose(file);
        fclose(temp);
        return 0;
    }
    size_t sajz = sizeof(Module);
    while (!feof(file)) {
        Module d;
        int succ = fread(&d, sajz, 1, file);
        if (succ) {
            if (!((d.id == d1 -> id) && (d.loc == d1 -> loc) \
            && (d.cell_loc == d1 -> cell_loc) && ((d.del == d1 -> del))\
            && (strcmp(d.name, d1 -> name) == 0))) {
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
    size_t sajz = sizeof(Module);
    Module obj;

    if ((end == 0) && (start == 0)) {
        fseek(file, 0, SEEK_END);
        int n = ftell(file)/sizeof(Module);
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
            printf("%d %s %d %d %d\n", obj.id, obj.name, obj.loc, obj.cell_loc, obj.del);
            i++;
        }
    }
    fclose(file);
    return 1;
}
