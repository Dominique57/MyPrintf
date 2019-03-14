//
// Created by domin on 13/03/2019.
//
#ifndef PRINTF_H
#define PRINTF_H
#include <stdarg.h>
#include <lzma.h>

typedef struct list list;
struct list {
    list *next;
    char *start;
    char *end;
    char isText;
};


void my_printf(char *s, ...);
size_t IsPrintableCharacter(char *s);
list *CreateList(char *s, size_t *argc);
void printString(list *l);
void UpdateList(list **curlp, char *cur, size_t length);
void ConvertToString(char *s, char *end, va_list variadic);
size_t UnsignedToString(unsigned long n, char s[]);
size_t SignedToString(long long n, char s[]);
void PrintChar(int c);
void PrintString(char *s);
size_t DoubleToString(double n, int precision, char s[]);

#endif //PRINTF_H
