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
list *CreateList(char *s, size_t *argc);
void FreeList(list *l);
void ConvertToString(char *s, char *end, va_list variadic, VarSetting *var);

// params :
// *s : string of given text
// ... : variables to define in the given string
void my_printf(char *s, ...)
{
    size_t argc = 0;
    list *l = CreateList(s, &argc);
    list * ref = l;
    va_list variadic;
    va_start(variadic, s);
    while(l)
    {
        if(l->var)
            ConvertToString(l->start, l->end, variadic, l->var);
        else
            write(STDOUT_FILENO, l->start, (l->end+1-l->start));
        l = l->next;
    }
    va_end(variadic);
    FreeList(ref);
}


// params :
// *s : begin of text to convert
// *argc : pointer to number counting the requested arguments
// return : pointer to created list
list *CreateList(char *s, size_t *argc)
{
    list *l = calloc(sizeof(list), 1);
    l->start = s;
    list *curl = l;
    char *cur = s;
    for (; *cur; ++cur)
    {
        if(*cur == '%' && *(cur+1) && *(cur+1) != '%')
        {
            VarSetting *var = NULL;
            size_t length2 = ReadData(cur+1, &var);
            if(length2 != 0)
            {
                UpdateList(&curl, cur, length2, var);
                cur += length2;
                (*argc)++;
            }
        }
    }
    curl->end = cur - 1;
    return l;
}

// params :
// **curl : pointer to pointer of current list
// *cur : crrent character analysed
// length : length of the new buffer to create
// *var : settings to save
void UpdateList(list **curl, char *cur, size_t length, VarSetting *var)
{
    //if current buffer is not untoched, close it
    if((*curl)->start != cur)
    {
        (*curl)->end=cur-1;
        (*curl)->next=calloc(sizeof(list), 1);
        *curl = (*curl)->next;
        (*curl)->start=cur;
    }
    //close buffer
    (*curl)->end=cur+length;
    (*curl)->var = var;
    //open new buffer only if end not reached
    if(*(cur+length+1) != '\0')
    {
        (*curl)->next = calloc(sizeof(list), 1);
        *curl = (*curl)->next;
        (*curl)->start = cur + length + 1;
    }
}

// param : list to free and eventually free its element
void FreeList(list *l)
{
    list * cur = l;
    while(cur)
    {
        l = cur->next;
        if(cur->var)
            FreeVarSetting(cur->var);
        free(cur);
        cur = l;
    }
}


// params:
// *s : begin of string to convert
// *end : end of string to convert
// variadic : current variadic element
// *var : pointer to variable settings
void ConvertToString(char *s, char *end, va_list variadic, VarSetting *var)
{
    while(!IsLetter(*(s+1)))
        s++;
    size_t length = end + 1 - s;
    switch (*(s+1))
    {
        case 'l':
            if(length >= 2)
            {
                if (*(s + 2) == 'u')
                    // unsigned long
                else if (*(s + 2) == 'd')
                    // long
                else if (*(s + 2) == 'f')
                    // double
            }
            else
                goto DEFAULT;
            break;
        case 'i':
        case 'd':
            // int
            break;
        case 's':
            // char*
            break;
        case 'c':
            // int
            break;
        case 'f':
            // double
            break;
        default:
            DEFAULT:
            //should not happen
            va_arg(variadic, double);
            write(STDOUT_FILENO, "?", 1);
            break;
    }
}
// params:
// *s : begin of string to convert
// *end : end of string to convert
// variadic : current variadic element
void ConvertToStringOld(char *s, char *end, va_list variadic)
{
    size_t length = end + 1 - s;
    char string[100] = {0};
    size_t count = 0;
    while(!IsLetter(*(s+1)))
        s++;
    switch (*(s+1))
    {
        case 'l':
            if(length >= 2)
            {
                if (*(s + 2) == 'u')
                    count = UnsignedToString(va_arg(variadic, unsigned long), string);
                else if (*(s + 2) == 'd')
                    count = SignedToString(va_arg(variadic, long), string);
                else if (*(s + 2) == 'f')
                    count = DoubleToString(va_arg(variadic, double), DEFAULT_PRECISION, string);
                else
                {
                    va_arg(variadic, double);
                    write(STDOUT_FILENO, "?", 1);
                }
            }
            break;
        case 'i':
        case 'd':
            count = SignedToString(va_arg(variadic, int), string);
            break;
        case 's':
            PrintString(va_arg(variadic, char*));
            break;
        case 'c':
            PrintChar(va_arg(variadic, int));
            break;
        case 'f':
            count = DoubleToString(va_arg(variadic, double), DEFAULT_PRECISION, string);
            break;
        default:
            //should not happen
            va_arg(variadic, double);
            write(STDOUT_FILENO, "?", 1);
            break;
    }
    if(string[0])
        write(STDOUT_FILENO, string, count);
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
    for (char *s = l->start; s <= l->end; ++s)
    {
        if(*s == ' ')
            printf("_");
        else
            printf("%c", *s);
    }
    printf("\n");
    printf("%s\n\n", ((l->var == NULL)?"isText":"isVar"));
}

#endif