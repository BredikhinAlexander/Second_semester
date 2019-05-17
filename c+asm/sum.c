#include <stdio.h>

extern int mysum(int a, int b);
int CSum(int a, int b);

int main()
{
    printf("%d\n", mysum(23, 56));
    
    return 0;
}


int CSum(int a, int b)
{
    return a + b;
}

