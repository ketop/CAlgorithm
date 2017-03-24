#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sort.h"




/* The argument k specifies the maximum integer in data, plus 1. */
int ctsort(int *data, int size, int k)
{
    int *counts,
        *temp;
    int i,
        j;

    if((counts = (int *)malloc(k *sizeof(int))) == NULL)
        return -1;
    /* temp与data有相同大小 */
    if((temp = (int *)malloc(size * sizeof(int))) == NULL)
    {
        free(counts);
        return -1;
    }
    for(i = 0; i < k; i++)
        counts[i] = 0;

    for(j = 0; j < size; j++)
        counts[data[j]] += 1;
    /* 调整系数 */
    for(i = 1; i < k; i++)
        counts[i] = counts[i] + counts[i - 1];
    /* 在temp中排序 */
    for(j = size - 1; j >= 0; j--)
    {
        temp[counts[data[j]] - 1] = data[j];
        counts[data[j]] = counts[data[j]] - 1;
    }
    memcpy(data, temp, size * sizeof(int));
    free(counts);
    free(temp);
    return 0;

}
