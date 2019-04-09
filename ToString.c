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

char IsEven(long n)
{
    return n % 2 == 0;
}

/// ///////////////////////////////
/// real to string functions
/// ///////////////////////////////

size_t UnsignedToBuf(unsigned long n, char buffer[], char PrintSign)
{
    size_t length = 0;
    for(;n !=0; n = n/10)
        buffer[length++] = (char)('0' + n % 10);
    if (PrintSign)
        buffer[length++] = '+';
    return length;
}

size_t SignedToBuf(long n, char buffer[], char PrintSign)
{
    size_t length = 0;
    long val = (n < 0)? -n: n;
    length = UnsignedToBuf((unsigned long)val, buffer, 0);
    if(n < 0)
        buffer[length++] = '-';
    else if (PrintSign)
        buffer[length++] = '+';
    return length;
}

size_t FloatingToBuf(double n, char buffer[], VarSetting *var)
{
    size_t length = 0;
    //integer part
    double val = (n < 0)? -n: n;
    long integer = (long)n;
    //decimal part
    int precision = var->precision + 1;
    int zeros = 0;
    unsigned long decimal = 0;
    double decimalD = val - (long)val;
    // count zeros and save floating point
    for(; precision > 0 && (long)(decimalD * 10) == 0;
        decimalD *= 10, decimalD -= (long) decimalD, --precision)
        zeros++;
    if(zeros != var->precision+1)
    {
        for (; precision > 0; --precision)
        {
            decimalD *= 10;
            decimal = decimal * 10 + (long) decimalD;
            decimalD -= (long) decimalD;
        }
        // adjust round value
        char toRound = (char) (decimal % 10);
        decimal /= 10;
        if (toRound >= 5)
            decimal++;
        length += UnsignedToBuf(decimal, buffer + length, 0);
        for (; zeros > 0; zeros--)
            buffer[length++] = '0';
        buffer[length++] = '.';
    }
    length += SignedToBuf((long)n, buffer + length, var->IsPrintingSign);
    return length;
}

/// ///////////////////////////////
/// VarSetting Format function
/// ///////////////////////////////

void CheckSignPlacement(VarSetting *var, char **buffer, char **curptr,
                        size_t *length, char IsInverted)
{
    char *buf = *buffer;
    if(IsInverted)
    {
        if(var->fillChar == '0' && IsSign(buf[(*length)-1]))
        {
            var->width--;
            *((*curptr)++) = buf[--(*length)];
        }
    }
    else
    {
        if(var->fillChar == '0' && IsSign(buf[0]))
        {
            var->width--;
            (*buffer)++;
        }
    }
}

void CopyValue(char *begin, char *buffer,
               size_t length, char IsInverted)
{
    if(IsInverted)
        for(;length>0;length--)
            *(begin++) = buffer[length-1];
    else
        for(size_t i = 0; i < length; i++)
            *(begin++) = buffer[i];
}


size_t FormatNumber(char buffer[], size_t length, char **s,
                    VarSetting *var, char IsInverted)
{
    size_t size = (var->width > length)? var->width: length;
    *s = malloc(size + 1);
    char *str = *s;
    if(var->width != 0 && var->width > length)
    {
        CheckSignPlacement(var, &buffer, &str, &length, IsInverted);
        size_t gap = var->width - length;
        char *start = str;
        if(var->IsAdjustingRight != 0)
            start = str + length;
        else
            str += gap;
        for(size_t i = 0; i < gap;++i)
            *(start++) = var->fillChar;
    }
    CopyValue(str, buffer, length, IsInverted);
    *(*s + size) = '\0';
    return size;
}

/// ///////////////////////////////
/// Calling functions for conversion and formating
/// ///////////////////////////////

size_t MySignedToStr(long n, char **s, VarSetting *var)
{
    char buffer[MAX_SIZE_SIGNED];
    size_t length = SignedToBuf(n, buffer, var->IsPrintingSign);
    if(length == 0)
        return 0;
    size_t size = FormatNumber(buffer, length, s, var, 1);
    return size;
}

size_t MyUnsignedToStr(unsigned long n, char **s, VarSetting *var)
{
    char buffer[MAX_SIZE_UNSIGNED];
    size_t length = UnsignedToBuf(n, buffer, var->IsPrintingSign);
    if(length == 0)
        return 0;
    size_t size = FormatNumber(buffer, length, s, var, 1);
    return size;
}

size_t MyFloatingToStr(double n, char **s, VarSetting *var)
{
    if(var->precision > MAX_SIZE_SIGNED)
        var->precision = MAX_SIZE_SIGNED;
    char buffer[MAX_SIZE_DOUBLE];
    size_t length = FloatingToBuf(n, buffer, var);
    if(length == 0)
        return 0;
    size_t size = FormatNumber(buffer, length, s, var, 1);
    return size;
}

/// ///////////////////////////////
/// Calling functions
/// ///////////////////////////////

// done
void CharToStr(va_list variadic, VarSetting *var, my_string *str)
{
    size_t size = ( var->width > 1)? var->width : 1;
    str->s = malloc(size * sizeof(char));
    for(size_t i = 0; i < size; ++i)
        str->s[i] = var->fillChar;
    char c = (char)va_arg(variadic, int);
    if(var->IsAdjustingRight == 1)
        str->s[0] = c;
    else
        str->s[size-1] = c;
    str->length = size;
    str->toFree = 1;
}

// done
void IntToStr(va_list variadic, VarSetting *var, my_string *str)
{
    char **result = &(str->s);
    size_t length = MySignedToStr(va_arg(variadic, int), result, var);
    str->length = length;
    str->toFree = 1;
}

// done
void UintToStr(va_list variadic, VarSetting *var, my_string *str)
{
    char **result = &(str->s);
    size_t length = MyUnsignedToStr(va_arg(variadic, unsigned int), result, var);
    str->length = length;
    str->toFree = 1;
}

// done
void LongToStr(va_list variadic, VarSetting *var, my_string *str)
{
    char **result = &(str->s);
    size_t length = MySignedToStr(va_arg(variadic, long), result, var);
    str->length = length;
    str->toFree = 1;
}

// done
void UlongToStr(va_list variadic, VarSetting *var, my_string *str)
{
    char **result = &(str->s);
    size_t length = MyUnsignedToStr(va_arg(variadic, unsigned long), result, var);
    str->length = length;
    str->toFree = 1;
}

void FloatToStr(va_list variadic, VarSetting *var, my_string *str)
{
    DoubleToStr(variadic, var, str);
}

void DoubleToStr(va_list variadic, VarSetting *var, my_string *str)
{
    char **result = &(str->s);
    size_t length = MyFloatingToStr(va_arg(variadic, double), result, var);
    str->length = length;
    str->toFree = 1;
}

// done
void StringToStr(va_list variadic, VarSetting *var, my_string *str)
{
    // TODO (set var setting)
    char *s = va_arg(variadic, char *);
    str->s = s;
    size_t length = 0;
    for(;*s;s++)
        length++;
    str->length = length;
    str->toFree = 0;
}

void PointerToStr(va_list variadic, VarSetting *var, my_string *str)
{
    char **result = &(str->s);
    size_t length = MySignedToStr(va_arg(variadic, int), result, var);
    str->length = length;
    str->toFree = 1;
}


void SpointerToStr(va_list variadic, VarSetting *var, my_string *str)
{
    char **result = &(str->s);
    size_t length = MySignedToStr(va_arg(variadic, int), result, var);
    str->length = length;
    str->toFree = 1;
}