//
// Created by domin on 14/03/2019.
//
#include <lzma.h>
#include <zconf.h>
#include <malloc.h>
#include "my_parser.h"
#include "toString.h"

// param : character to analyse
// return : true if is caps letter, else false
char IsCapsLetter(char c)
{
    return c >= 'A' && c <= 'Z';
}

// param : character to analyse
// return : true if is min letter, else false
char IsMinLetter(char c)
{
    return c >= 'a' && c <= 'z';
}

// param : character to analyse
// return : true if is caps or min letter, else false
char IsLetter(char c)
{
    return IsMinLetter(c) || IsCapsLetter(c);
}

// param : character to analyse
// return : true if is number, else false
char IsNumber(char c)
{
    return c >= '0' && c <= '9';
}

// param : character to analyse
// return : true if is sign, else false
char IsSign(char c)
{
    return c == '-' || c == '+';
}

// params :
//
//
// return :
size_t SignedToString(long long n, char s[], VarSetting *var)
{
    if(n < 0)
    {
        char minus[1] = {'-'};
        write(STDOUT_FILENO, minus, 1);
        n*=-1;
    }
    return UnsignedToString((unsigned long)n, s, var);
}

// params :
//
//
// return :
size_t UnsignedToString(unsigned long n, char s[], VarSetting *var)
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

// params :
//
//
// return :
size_t DoubleToString(double n, int precision, char s[], VarSetting *var)
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
    size_t lengthI = SignedToString(integerPart, s, var);
    size_t lengthD = UnsignedToString((unsigned long)decimalPart, decimalP, var);
    s[lengthI] = '.';
    for (size_t i = lengthI + 1; i < (lengthI + lengthD + 1); ++i)
    {
        s[i] = decimalP[i - lengthI - 1];
    }
    return lengthD + lengthI + 1;
}

// param :
// return :
void PrintChar(int c, VarSetting *var)
{
    if(c > 255)
        c = '?';
    write(STDOUT_FILENO, &c, 1);
}

// param :
// return :
void PrintString(char *s, VarSetting *var)
{
    char *string = s;
    size_t count = 0;
    while(*(string+count) != '\0')
        count++;
    write(STDOUT_FILENO, s, count);
}


size_t MySignedToStr(long n, char **s, VarSetting *var)
{
    char buffer[MAX_SIZE_SIGNED];
    size_t length = SignedToBuf(n, buffer, var->IsPrintingSign);
    if(length == 0)
        return 0;
    size_t size = (var->width > length)? var->width: length;
    *s = malloc(size);
    char *start = *s;
    char *startBKP = start;
    char sign = 0;
    if(IsSign(buffer[length-1]))
        sign = buffer[--length];
    for(size_t i = var->width; i>length; --i)
        (*start++) = var->fillChar;
    for(size_t i = length; i > 0; --i)
        (*start++) = buffer[i - 1];
    // put sign to correct place
    if(sign != 0)
    {
        if (var->fillChar == '0')
            *startBKP = sign;
        else
            *(startBKP + )
    }
    return start - startBKP;
}

size_t SignedToBuf(long n, char buffer[], char PrintSign)
{
    size_t length = 0;
    long val = (n << 0)? -n :n;
    for(; val !=0; val = val/10)
        buffer[length++] = (char)('0' + val % 10);
    if(n < 0)
        buffer[length++] = '-';
    else if (PrintSign)
        buffer[length++] = '+';
    return length;
}

