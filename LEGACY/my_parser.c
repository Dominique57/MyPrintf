//
// Created by domin on 24/03/2019.
//
#include <malloc.h>
#include "my_parser.h"
#include "toString.h"

size_t ReadNumber(char *s, size_t *number);
size_t ReadParameter(char *s, VarSetting *var);
size_t ReadParameter(char *s, VarSetting *var);
size_t ReadFlags(char *s, VarSetting *var);
size_t ReadWidth(char *s, VarSetting *var);
size_t ReadPrecision(char *s, VarSetting *var);
size_t ReadType(const char *s);

// params :
// *s : string to read settings from
// **var : pointer of pointer to store settings in
//       : if no settings read, pointer is set to null
// return : length of read settings
size_t ReadData(char *s, VarSetting **var)
{
    char *begin = s;
    *var = InitVarSetting();
    s += ReadParameter(s, *var);
    s += ReadFlags(s, *var);
    s += ReadWidth(s, *var);
    s += ReadPrecision(s, *var);
    s += ReadType(s);
    size_t length = s - begin;
    if(length == 0)
    {
        free(*var);
        *var = NULL;
        return 0;
    }
    return length;
}



// -------------- dependencies --------------


// params :
// *s : string to reads number from
// *number : number where read number will be stored
// return : length of read number
size_t ReadNumber(char *s, size_t *number)
{
    char * cur = s;
    for(;*cur && IsNumber(*cur);++cur)
    {
        *number = (*number * 10) + (*cur - '0');
    }
    return cur - s;
}

// params :
// *s : string to reads setting from
// *var : VarSetting to store read setting in
// return : length of read setting
size_t ReadParameter(char *s, VarSetting *var)
{
    size_t number = 0, length = ReadNumber(s, &number);
    if(length != 0 && *(s + length) && *(s + length) == '$')
    {
        var->varPosition = (unsigned int)number;
        printf("A parameter has been read\n");
        return length + 1;
    }
    return 0;
}

// params :
// *s : string to reads setting from
// *var : VarSetting to store read setting in
// return : length of read setting
size_t ReadFlags(char *s, VarSetting *var)
{
    switch (*s)
    {
        case '0':
            var->fillChar = '0';
            // replace sapce with zero
            break;
        case '+':
            var->IsPrintingSign = 1;
            // print sign of number
            break;
        case '-':
            var->IsAdjustingRight = 1;
            // adjust padding to the right
            break;
        default:
            return 0;
    }
    // read other data
    if(*(s+1))
    {
        return 1 + ReadFlags(s+1, var);
    }
    else
        return 1;
}

// params :
// *s : string to reads setting from
// *var : VarSetting to store read setting in
// return : length of read setting
size_t ReadWidth(char *s, VarSetting *var)
{
    size_t number = 0, length = ReadNumber(s, &number);
    if(length != 0)
    {
        var->width = (unsigned int)number;
        //set width
        return length;
    }
    else
        return 0;
}

// params :
// *s : string to reads setting from
// *var : VarSetting to store read setting in
// return : length of read setting
size_t ReadPrecision(char *s, VarSetting *var)
{
    if (*s == '.')
    {
        s++;
        size_t number = 0, length = ReadNumber(s, &number);
        //set precision
        var->precision = (unsigned int) number;
        return 1 + length;
    }
    return 0;
}

/*
// WIP
void ReadLength(char *s)
{

}
*/

//only used for offset
// params :
// *s : string to reads setting from
// return : length of read setting
size_t ReadType(const char *s)
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
            else
                return 0;
            break;
        default:
            return 0;
    }
    return 0;
}

// return : a correclty initiaized VarSetting struct
VarSetting *InitVarSetting()
{
    VarSetting *p = malloc(sizeof(VarSetting));
    p->varPosition = 0;
    p->fillChar = ' ';
    p->IsPrintingSign = 0;
    p->IsAdjustingRight = 0;
    p->width = 0;
    p->precision = 6;
    return p;
}

// params :
// *var : VarSetting be freed
void FreeVarSetting(VarSetting *var)
{
    free(var);
}