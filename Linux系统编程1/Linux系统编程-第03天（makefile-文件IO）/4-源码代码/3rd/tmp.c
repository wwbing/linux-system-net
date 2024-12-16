#include <stdio.h>

int main(void)
{
    int a = 3;

#ifdef DEBUG
    printf("hello itcast\n");
#endif

    printf("hello world\n");

    return 0;
}
