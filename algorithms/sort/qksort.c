#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "sort.h"



/* 额外使用了插入排序 */

static int compare_int(const void *int1, const void *int2)
{
    if(*(const int *)int1 > *(const int *)int2)
        return 1;
    else if(*(const int *)int1 < *(const int *)int2)
        return -1;
    else
        return 0;
}

static int partition(void *data, int esize, int i, int k, int (*compare)(const void *key1, const void *key2))
{
    char *a = data;
    void *pval,
         *temp;
    int r[3];
    if((pval = malloc(esize)) == NULL)
        return -1;
    if((temp = malloc(esize)) == NULL)
    {
        free(pval);
        return -1;
    }
    /* Use the median-of-three method to find the partition value. */
    r[0] = (rand() % (k - i + 1)) + i;
    r[1] = (rand() % (k - i + 1)) + i;
    r[2] = (rand() % (k - i + 1)) + i;

    /* 对r进行插入排序 */
    issort(r, 3, sizeof(int), compare_int);
    /* 选择中值 */
    memcpy(pval, &a[r[1] * esize], esize);
    i--;
    k++;
    /* 分治思想来了 */
    while(1)
    {
        /* 现将右侧与pval进行比较 */
        do {
            k--;

            /* 如果a[k]大于pval则不进行改变 */
        }while(compare(&a[k * esize], pval) > 0);

        /* 再将左侧与pval进行比较 */
        do {
            i++;
            /* 如果a[i]小于pval则不进行改变 */
        }while(compare(&a[i * esize], pval) < 0);

        /* 此时存在关系: a[i] > pval > a[k] */
        if( i >= k )
        {
            break;
        }
        else
        {
            /* swap操作 */
            /* 现将a[i]拷入到temp，再将a[k]拷到a[i],最后将temp烤到a[k] */
            memcpy(temp, &a[i * esize], esize);
            memcpy(&a[i * esize], &a[k * esize], esize);
            memcpy(&a[k * esize], temp, esize);
        }
    }
    free(pval);
    free(temp);
    return k;
}

int qksort(void *data, int size, int esize, int i, int k, int (*compare)(const void *key1, const void *key2))
{
    int j;
    while(i < k)
    {
        /* 先进行一次partition得到中值索引 */
        if((j = partition(data, esize, i, k, compare)) < 0)
            return -1;
        /* 进行快排，左侧快排 */
        if(qksort(data, size, esize, i, j, compare) < 0)
            return -1;
        /* 右侧快排 */
        i = j + 1;
    }
    return 0;
}

