//
// Created by domin on 13/03/2019.
//
#include <stdarg.h>
#include <lzma.h>
#include <unistd.h>
#include <malloc.h>
#include "printf.h"


void my_printf(char *s, ...)
{
    size_t argc = 0;
    list *l = CreateList(s, &argc);
    va_list variadic;
    va_start(variadic, argc);
    while(l)
    {
        if(!l->isText)
            ConvertToString(l->start, l->end, variadic);
        else
            write(STDOUT_FILENO, l->start, (l->end+1-l->start));
        l = l->next;
    }
    va_end(variadic);
}

list *CreateList(char *s, size_t *argc)
{
    list *l = calloc(sizeof(list), 1);
    l->start = s;
    list *curl = l;
    char *cur = s;
    for (; *cur; ++cur)
    {
        if(*cur == '%')
        {
            size_t length;
            if((length = IsPrintableCharacter(cur+1)) != 0)
            {
                UpdateList(&curl, cur, length);
                cur+=length;
                (*argc)++;
            }
        }
    }
    curl->end = cur - 1;
    return l;
}

void UpdateList(list **curl, char *cur, size_t length)
{
    if((*curl)->start != cur)
    {
        (*curl)->isText = 1;
        (*curl)->end=cur-1;
        (*curl)->next=calloc(sizeof(list), 1);
        *curl = (*curl)->next;
        (*curl)->start=cur;
    }
    (*curl)->end=cur+length;
    if(*(cur+length+1)!='\0')
    {
        (*curl)->next = calloc(sizeof(list), 1);
        *curl = (*curl)->next;
        (*curl)->start = cur + length + 1;
    }
}

size_t IsPrintableCharacter(char *s)
{
    if(*s == '\0')
        return 0;
    char c = *s;
    switch(c)
    {
        case 'd':
        case 'i':
        case 'o':
        case 'x':
        case 'X':
        case 'u':
        case 'c':
        case 's':
        case 'f':
        case 'e':
        case 'E':
        case 'g':
        case 'G':
        case 'p':
            return 1;
        case 'l':
            if(*(s+1))
            {
                switch (*(s+1))
                {
                    case 'u':
                    case 'd':
                    case 'f':
                        return 2;
                    default:
                        break;
                }
            }
        default:
            return 0;
    }

}

void printString(list *l)
{
    for (char *s = l->start; s <= l->end; ++s)
    {
        printf("%c", *s);
    }
    printf("\n");
    printf("%s\n\n", ((l->isText)?"isText":"isVar"));
}

void ConvertToString(char *s, char *end, va_list variadic)
{
    size_t length = end + 1 - s;
    char string[100] = {0};
    size_t count = 0;
    int precision = 6;
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
                    count = DoubleToString(va_arg(variadic, double), precision, string);
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
            count = DoubleToString(va_arg(variadic, double), precision, string);
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

size_t SignedToString(long long n, char s[])
{
    if(n < 0)
    {
        char minus[1] = {'-'};
        write(STDOUT_FILENO, minus, 1);
        n*=-1;
    }
    return UnsignedToString((unsigned long)n, s);
}

size_t UnsignedToString(unsigned long n, char s[])
{
    int maxSize = 32;
    int pos=maxSize-1;
    for(; n>0; --pos, n/=10)
        s[pos] = (char)('0' + n%10);
    size_t i = 0;
    for (; pos + 1 < maxSize; ++pos, ++i)
    {
        s[i] = s[pos+1];
        s[pos+1] = '\0';
    }
    if(i == 0)
        s[i++] = '0';
    return i;
}

size_t DoubleToString(double n, int precision, char s[])
{
    long long integerPart = (long long)n;
    long precisionFactor = 1;
    while (precision-- > 0)
        precisionFactor*=10;
    long long decimalPart = (long long)(n - integerPart) * precisionFactor;
    if(decimalPart<0)
        decimalPart*=-1;
    while(decimalPart%10 == 0 && decimalPart != 0)
        decimalPart/=10;
    char decimalP[32] = {0};
    size_t lengthI = SignedToString(integerPart, s);
    size_t lengthD = UnsignedToString((unsigned long)decimalPart, decimalP);
    s[lengthI] = '.';
    for (size_t i = lengthI + 1; i < (lengthI + lengthD + 1); ++i)
    {
        s[i] = decimalP[i - lengthI - 1];
    }
    return lengthD + lengthI + 1;
}

void PrintChar(int c)
{
    if(c > 255)
        return;
    char cara = (char)c;
    char carac[1] = {cara};
    write(STDOUT_FILENO, carac, 1);
}

void PrintString(char *s)
{
    char *string = s;
    size_t count = 0;
    while(*(string+count) != '\0')
        count++;
    write(STDOUT_FILENO, s, count);
}
