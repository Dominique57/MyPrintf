//
// Created by domin on 24/03/2019.
//

#ifndef MY_PARSER_H
#define MY_PARSER_H

#include <lzma.h>
#include <stdarg.h>

typedef struct my_string my_string;
struct my_string{
    char *s;
    size_t length;
    char toFree;
};

void InitMyStringValue(my_string *str);


typedef enum Var_t Var_t;
enum Var_t{None, Char, Int, UInt, Long, ULong, Float, Double, String, Pointer, SmallPointer};

typedef struct VarSetting VarSetting;
struct VarSetting {
    unsigned int varPosition; // position begins from 1
    char fillChar;
    char IsPrintingSign;
    char IsAdjustingRight;
    unsigned int width;
    unsigned int precision;
    Var_t type;
};

VarSetting *InitVarSetting();
void InitVarSettingValue(VarSetting *p);
void FreeVarSetting(VarSetting *var);


size_t ReadSettings(char *s, VarSetting *var);
size_t ReadData(va_list variadic, VarSetting *var, my_string *str);


#endif //MY_PARSER_H
