#ifndef MODULES_H_INCLUDED
#define MODULES_H_INCLUDED

typedef struct Module {
    int id;
    char name[30];
    int loc;
    int cell_loc;
    int del;
} Module;

#endif // MODULES_H_INCLUDED
