#include <malloc.h>
#include "printf.h"
#include "toString.h"

#include <stdio.h>

int main()
{

    VarSetting *var = InitVarSetting();

    long n = 123456;
    char *s = NULL;
    size_t length = MySignedToStr(n, &s, var);
    return 0;
    long l = 2345678912345678912;
    my_printf("my_printf %i %05d %ld %lu %lf %c %s\n",
               -1, 0, l, l, -10.125, 'c', "Hello World!");
    return 0;
}