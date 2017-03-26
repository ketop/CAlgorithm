#include <stdio.h>

#include "numerial.h"

double f(double x)
{
    return x*x*x-x*x-3*x+1.8;
}

double g(double x)
{
    return 3*x*x-2*x-3;
}

int main()
{
    double x[100];
    x[0] = -2;
    int n = 100;
    root(f, g, x, &n, 0.0001);
    printf("iterate %d times\n", n);
    int i;
    for(i = 0; i < n; i++)
    {
        printf("%d: %f\n", i, x[i]);
    }
    return 0;
}
