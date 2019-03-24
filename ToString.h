//
// Created by domin on 14/03/2019.
//

#ifndef TOSTRING_H
#define TOSTRING_H

#include "my_parser.h"

#define MAX_SIZE SIGNED
#define MAX_SIZE_UNSIGNED
#define MAX_SIZE_DOUBLE

char IsCapsLetter(char c);
char IsMinLetter(char c);
char IsNumber(char c);
char IsLetter(char c);



size_t SignedToString(long long n, char s[], VarSetting *var);
size_t UnsignedToString(unsigned long n, char s[], VarSetting *var);
size_t DoubleToString(double n, int precision, char s[], VarSetting *var);
void PrintChar(int c, VarSetting *var);
void PrintString(char *s, VarSetting *var);

#endif //TOSTRING_H
