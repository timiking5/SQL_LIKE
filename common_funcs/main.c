#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "databases.h"

int init_db(char* filename);
int insert(struct my_struct * obj, char* filename);
int update(char* filename, struct my_struct* old, struct my_struct* new);
int delete(char* filename, struct my_struct* obj);
int select(char* filename, int start, int end);
int proc_quer(char* command);
int solution(char * filename);




int main() {

    // В этом файле находятся функции, которые являются
    // общими для всех файлов, а их не так уж и много,
    // потому что во многом исполнение зависит от ввода

    return 0;
}

// If there is no such file for db it will be created
int init_db(char* filename) {
    int err = 1;
    FILE * file = fopen(filename, "ab");
    if (!file)
        err = 0;
    if (file) fclose(file);
    return err;
}

// inserting in the end of a db
int insert(struct my_struct * obj, char* filename) {
    int err = 1;
    FILE * file = fopen(filename, "rb+");
    if (!file) {
        err = 0;
    } else {
        err = fwrite(obj, sizeof(obj), 1, file);
    }
    if (file) fclose(file);
    return err;
}

int proc_quer(char* command) {
    char option[][7] = {"insert", "update", "select", "delete"};
    int opt = -1;
    int i;
    printf("command: %s", command);
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

int delete(char* filename, struct my_struct * d1) {
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
    int n = ftell(file)/sizeof(struct my_struct);
    rewind(file);
    if (n == 0) {
        fclose(file);
        fclose(temp);
        return 0;
    }
    size_t sajz = sizeof(struct my_struct);
    while (!feof(file)) {
        struct my_struct d;
        int succ = fread(&d, sajz, 1, file);
        if (succ) {
            if (d.a != d1 -> a) {
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

int update(char* filename, struct my_struct * old, struct my_struct * new) {
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
    int n = ftell(file)/sizeof(struct my_struct);
    rewind(file);
    if (n == 0) {
        fclose(temp);
        fclose(file);
        return 0;
    }
    size_t sajz = sizeof(struct my_struct);
    while (!feof(file)) {
        struct my_struct d;
        int succ = fread(&d, sajz, 1, file);
        if (succ) {
            if (d.a != old -> a) {
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
            struct my_struct obj;
            resp = proc_ins(word, &obj);
            if (resp) {
                sucs = insert(&obj, filename);
                if (!sucs) printf("n/a\n");
            } else {
                printf("n/a\n");
            }
        } else if (ans == 1) {
            struct my_struct old, new;
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
            struct my_struct obj;
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

int select(char* filename, int start, int end) {
    if (start > end)
        return 0;
    FILE * file = fopen(filename, "rb");
    if (!file) {
        return 0;
    }
    int i = 0;
    size_t sajz = sizeof(struct my_struct);
    struct my_struct obj;

    if ((end == 0) && (start == 0)) {
        fseek(file, 0, SEEK_END);
        int n = ftell(file)/sizeof(struct my_struct);
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
            printf("%d\n", obj.a);
            i++;
        }
    }
    fclose(file);
    return 1;
}
