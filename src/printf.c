//
// Created by domin on 13/03/2019.
//
#include <stdarg.h>
#include <lzma.h>
#include <zconf.h>
#include <malloc.h>
#include "printf.h"
#include "toString.h"
#include "my_parser.h"

void UpdateList(list **curlp, char *cur, size_t length, VarSetting *var);
list *CreateList(char *s, va_list variadic);
void FreeList(list *l);
void ConvertToString(char *s, char *end, va_list variadic, VarSetting *var);
size_t ReadVariableValue(char **s, list *l, va_list variadic);
size_t GetTotalLength(list *l);
char CopyListToBuff(char buf[], list *l, size_t *length);


list *InitList()
{
    list *l = calloc(sizeof(list), 1);
    return l;
}

void my_printf(char *s, ...)
{
    va_list variadic;
    va_start(variadic, s);
    list *l = CreateList(s, variadic);
    va_end(variadic);
    size_t length = GetTotalLength(l);
    char final[length];
    char success = CopyListToBuff(final, l, &length);
    FreeList(l);
    if (write(STDOUT_FILENO, final, length) == -1 || success == 0)
        return; // error
}

size_t GetTotalLength(list *l)
{
    if(!l)
        return 0;
    if(l->length == 0)
    {
        free(l);
        return 0;
    }
    size_t length = l->length;
    list *parent = l;
    list *cur = l->next;
    while(cur)
    {
        if(cur->length != 0)
        {
            length += cur->length;
            parent = cur;
            cur = cur->next;
        }
        else
        {
            parent->next = cur->next;
            free(cur);
            cur = parent->next;
        }
    }
    return length;
}

char CopyListToBuff(char buf[], list *l, size_t *length)
{
    size_t i = 0;
    for(;l;l=l->next)
    {
        char *end = l->start + l->length;
        for(char *start = l->start; start < end; ++start)
        {
            buf[i++] = *start;
        }
        if(l->toFree)
            free(l->start);
    }
    return !(length && i > *length);
}

list *CreateList(char *s, va_list variadic)
{
    list *l = InitList();
    l->start = s;
    list *curl = l;
    char *cur = s;
    for (; *cur; ++cur)
    {
        if (*cur == '%' && *(cur + 1))
        {
            if (*(cur + 1) == '%')
            {
                if (cur != curl->start)
                {
                    curl->length = cur - curl->start;
                    list *newl = InitList();
                    curl->next = newl;
                    curl = newl;
                }
                curl->start = cur + 1;
                cur++;
            }
            else
            {
                list *backup = curl;
                if (cur != curl->start)
                {
                    curl->length = cur - curl->start;
                    list *newl = InitList();
                    curl->next = newl;
                    curl = newl;
                }
                //read variable value (as str) and put in list
                size_t success = ReadVariableValue(&cur, curl, variadic);
                if(!success)
                {
                    free(curl);
                    curl = backup;
                    curl->next = NULL;
                    curl->length = 0;
                }
                else
                {
                    //buffer fermé par sous fonction, ouvrir un nouveau
                    list *newl = InitList();
                    curl->next = newl;
                    curl = newl;
                    curl->start = cur + 1;
                }
            }
        }
    }
    // close buffer if opend
    // if buffer has length = 0, it will be deleted in another function afterwards
    curl->length = cur - curl->start;
    return l;
}

size_t ReadVariableValue(char **s, list *l, va_list variadic)
{
    char *cur = (*s) + 1;
    VarSetting var;
    InitVarSettingValue(&var);
    size_t length = ReadSettings(cur, &var);
    *s += length;
    if(var.varPosition == 0)
    {
        // position is default
    }
    my_string str;
    InitMyStringValue(&str);
    size_t success = ReadData(variadic, &var, &str); // returns 1 on success
    l->start = str.s;
    l->length = str.length;
    l->toFree = str.toFree;
    return success;
}

// param : list to free and eventually free its element
void FreeList(list *l)
{
    list * cur = l;
    while(cur)
    {
        l = cur->next;
        free(cur);
        cur = l;
    }
}

#ifdef MY_PRINTF_DEB
// all debugging functions here

void printList(list *l)
{
    for(;l;l=l->next)
        printString(l);
}

void printString(list *l)
{
    printf("\n:");
    char* end = l->start + l->length;
    for (char *s = l->start; s < end; ++s)
    {
        if(*s == ' ')
            printf("_");
        else
            printf("%c", *s);
    }
    (l->toFree)? printf("Var"): printf("NotVar");
}

#endif