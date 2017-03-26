#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "numerial.h"


/*n指定有n个(x,fx)*/
/* 插值多项式的阶数完全由已知的点的个数有关 */
int interpol (const double *x, const double *fx, int n, double *z, double *pz, int m)
{
    double term,
           *table,
           *coeff;
    int i,
        j,
        k;
    /* 两个动态分配的内存 */
    if((table = (double *)malloc(sizeof(double) * n)) == NULL)
        return -1;
    if((coeff = (double *)malloc(sizeof(double) * n)) == NULL)
    {
        free(table);
        return -1;
    }
    /* (x,fx)为已知点，fx=f(x)拷贝 */
    memcpy(table, fx, sizeof(double) * n);
    coeff[0] = table[0];
    /* 已知点的个数n, 多项式有n项  */
    for(k = 1; k < n; k++)
    {
        for(i = 0; i < n - k; i++)
        {
            /* 除数需要注意 */
            j = i + k;
            table[i] = (table[i + 1] - table[i]) / (x[j] - x[i]);
        }
        coeff[k] = table[0];
    }
    free(table);

    /* m:要估计m个值 */
    for(k = 0; k < m; k++)
    {
        /* 对每个要插入的值代入到公式中 */
        pz[k] = coeff[0];//常数项
        for(j = 1; j < n; j++)
        {
            /* 系数项 */
            term = coeff[j];
            for(i = 0; i < j; i++)
            {
                /* 计算项 */
                term = term * (z[k] - x[i]);
            }
            /* 多项式相加 */
            pz[k] = pz[k] + term;
        }
    }
    free(coeff);
    return 0;
}
