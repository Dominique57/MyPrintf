#include "printf.h"

int main()
{
    long l = 12345678912345678912;
    my_printf("my_printf %i %d %ld %lu %lf %c %s",
               -1, 0, l, l, -10.125, 'c', "Hello World!");
    return 0;
}