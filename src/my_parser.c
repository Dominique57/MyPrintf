//
// Created by domin on 24/03/2019.
//
#include <malloc.h>
#include "printf.h"
#include "my_parser.h"
#include "toString.h"

size_t ReadNumber(char *s, size_t *number);
size_t ReadParameter(char *s, VarSetting *var);
size_t ReadParameter(char *s, VarSetting *var);
size_t ReadFlags(char *s, VarSetting *var);
size_t ReadWidth(char *s, VarSetting *var);
size_t ReadPrecision(char *s, VarSetting *var);
size_t ReadType(char *s, VarSetting *var);

// params :
// *s : string to read settings from
// **var : pointer of pointer to store settings in
//       : if no settings read, pointer is set to null
// return : length of read settings
size_t ReadSettings(char *s, VarSetting *var)
{
    char *begin = s;
    s += ReadParameter(s, var);
    s += ReadFlags(s, var);
    s += ReadWidth(s, var);
    s += ReadPrecision(s, var);
    s += ReadType(s, var);
    size_t length = s - begin;
    return length;
}

size_t ReadData(va_list variadic, VarSetting *var, my_string *str)
{
    switch(var->type)
    {
        case Char:
            CharToStr(variadic, var, str);
            break;
        case Int:
            IntToStr(variadic, var, str);
            break;
        case UInt:
            UintToStr(variadic, var, str);
            break;
        case Long:
            LongToStr(variadic, var, str);
            break;
        case ULong:
            UlongToStr(variadic, var, str);
            break;
        case Float:
            FloatToStr(variadic, var, str);
            break;
        case Double:
            DoubleToStr(variadic, var, str);
            break;
        case String:
            StringToStr(variadic, var, str);
            break;
        case Pointer:
            PointerToStr(variadic, var, str);
            break;
        case SmallPointer:
            SpointerToStr(variadic, var, str);
            break;
        case None:
        default:
            return 0; // fail
    }
    return 1; // success
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
size_t ReadType(char *s, VarSetting *var)
{
    if(*s == '\0')
        return 0;
    char c = *s;
    switch(c)
    {
        case 'd':
        case 'i':
            var->type = Int;
            return 1;
        case 'o':
            return 1;
        case 'x':
            var->type = SmallPointer;
            return 1;
        case 'X':
            var->type = Pointer;
            return 1;
        case 'u':
            return 1;
        case 'c':
            var->type = Char;
            return 1;
        case 's':
            var->type = String;
            return 1;
        case 'f':
            var->type = Float;
            return 1;
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
                        var->type = Long;
                        return 2;
                    case 'd':
                        var->type = ULong;
                        return 2;
                    case 'f':
                        var->type = Double;
                        return 2;
                    default:
                        return 0;
                }
            }
            else
                return 0;
        default:
            return 0;
    }
}


//////////////////////////////////////////////////
/// VarSetting functions
//////////////////////////////////////////////////
// set default value for my_string struct
void InitMyStringValue(my_string *str)
{
    str->s = NULL;
    str->length = 0;
    str->toFree = 0;
}

// return : a correclty initiaized VarSetting struct
VarSetting *InitVarSetting()
{
    VarSetting *p = malloc(sizeof(VarSetting));
    InitVarSettingValue(p);
    return p;
}

// set default value for VarSetting struct
void InitVarSettingValue(VarSetting *p)
{
    p->varPosition = 0;
    p->fillChar = ' ';
    p->IsPrintingSign = 0;
    p->IsAdjustingRight = 0;
    p->width = 0;
    p->precision = 6;
    p->type = None;
}

// params :
// *var : VarSetting be freed
void FreeVarSetting(VarSetting *var)
{
    free(var);
}