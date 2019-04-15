//
// Created by domin on 14/03/2019.
//

#ifndef TOSTRING_H
#define TOSTRING_H

#include "my_parser.h"

#define MAX_SIZE_SIGNED 20 //19 + sign
#define MAX_SIZE_UNSIGNED 20
#define MAX_SIZE_DOUBLE 50

// TODO DEL
size_t FloatingToBuf(double n, char buffer[], VarSetting *var);


char IsCapsLetter(char c);
char IsMinLetter(char c);
char IsNumber(char c);
char IsLetter(char c);
char IsSign(char c);

size_t MySignedToStr(long n, char **s, VarSetting *var);
size_t MyUnsignedToStr(unsigned long n, char **s, VarSetting *var);

void CharToStr(va_list variadic, VarSetting *var, my_string *str);
void IntToStr(va_list variadic, VarSetting *var, my_string *str);
void UintToStr(va_list variadic, VarSetting *var, my_string *str);
void LongToStr(va_list variadic, VarSetting *var, my_string *str);
void UlongToStr(va_list variadic, VarSetting *var, my_string *str);
void FloatToStr(va_list variadic, VarSetting *var, my_string *str);
void DoubleToStr(va_list variadic, VarSetting *var, my_string *str);
void StringToStr(va_list variadic, VarSetting *var, my_string *str);
void PointerToStr(va_list variadic, VarSetting *var, my_string *str);
void SpointerToStr(va_list variadic, VarSetting *var, my_string *str);

/*
size_t SignedToString(long long n, char s[], VarSetting *var);
size_t UnsignedToString(unsigned long n, char s[], VarSetting *var);
size_t DoubleToString(double n, int precision, char s[], VarSetting *var);
void PrintChar(int c, VarSetting *var);
void PrintString(char *s, VarSetting *var);

size_t MySignedToStr(long n, char **s, VarSetting *var);
// input buffer should be able to handle every character og given value
size_t SignedToBuf(long n, char buffer[], char PrintSign);

size_t MyUnSignedToStr();
size_t MyDoubleToStr();
size_t MyCharToStr();
size_t MyStringToStr();
size_t My__ToStr();
 */

#endif //TOSTRING_H
