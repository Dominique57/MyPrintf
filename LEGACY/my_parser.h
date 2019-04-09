//
// Created by domin on 24/03/2019.
//

#ifndef MY_PARSER_H
#define MY_PARSER_H

#include <lzma.h>

typedef struct VarSetting VarSetting;
struct VarSetting {
    unsigned int varPosition; // position begins from 1
    char fillChar;
    char IsPrintingSign;
    char IsAdjustingRight;
    unsigned int width;
    unsigned int precision;
};

VarSetting *InitVarSetting();
void FreeVarSetting(VarSetting *var);


size_t ReadData(char *s, VarSetting **var);


#endif //MY_PARSER_H
