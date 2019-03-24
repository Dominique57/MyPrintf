#include <malloc.h>
#include "printf.h"
#include "toString.h"

int main()
{
    long l = 2345678912345678912;
    my_printf("my_printf %i %05d %ld %lu %lf %c %s\n",
               -1, 0, l, l, -10.125, 'c', "Hello World!");
    return 0;
}