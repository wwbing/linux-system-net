#include <stdio.h>
#include "add.h"
#include "sub.h"
#include "mul.h"
#include "div.h"

int main(void)
{
    int x = 15;
    int y = 5;

    printf("x + y = %d\n", add(x, y));
    printf("x - y = %d\n", sub(x, y));
    printf("x * y = %d\n", mul(x, y));
    printf("x / y = %d\n", div(x, y));

    printf("hello world\n");

    return 0;
}
