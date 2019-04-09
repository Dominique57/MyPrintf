//
// Created by domin on 13/03/2019.
//
#ifndef PRINTF_H
#define PRINTF_H

#define MY_PRINTF_DEB
#define DEFAULT_PRECISION 6

#include <stdarg.h>
#include <lzma.h>
#include "toString.h"
#include "my_parser.h"


typedef struct list list;
struct list {
    list *next;
    char *start;
    char *end;
    VarSetting *var;
};

void my_printf(char *s, ...);



#ifdef MY_PRINTF_DEB
// all debugging functions here
void printList(list *l);
void printString(list *l);
#endif

#endif //PRINTF_H
