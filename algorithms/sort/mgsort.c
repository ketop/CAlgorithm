#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sort.h"

/* 将data的两块数据排序融合 */
static int merge(void *data, int esize, int i, int j, int k, int (*compare)(const void *key1, const void *key2))
{
    char *a = data,
         *m;
    int ipos,
        jpos,
        mpos;

    ipos = i;
    jpos = j+1;
    mpos = 0;
    /* 开辟与data同样大小的空间 */
    if((m = (char *)malloc(esize * (k - i) + 1)) == NULL)
        return -1;

    /* 左右两边分治 */
    while( ipos <= j || jpos <= k )
    {
        /* 如果左侧数据已完成遍历，则剩下的按序拷贝 */
        if(ipos > j)
        {
            while(jpos <= k)
            {
                memcpy(&m[mpos * esize], &a[jpos * esize], esize);
                jpos++;
                mpos++;
            }
            /* 直接返回外层while循环 */
            continue;
        }
        /* 如果右侧数据已完成遍历，则剩下的按序拷贝 */
        else if(jpos > k)
        {
            while(ipos <= j)
            {
                memcpy(&m[mpos * esize], &a[ipos * esize], esize);
                ipos++;
                mpos++;
            }
            continue;
        }

        /* 如果数据由jpos分为两侧，则进行比较 */
        /* 将较小值放入m中 */
        if(compare(&a[ipos * esize], &a[jpos * esize]) < 0)
        {
            memcpy(&m[mpos * esize], &a[ipos * esize], esize);
            ipos++;
            mpos++;
        }
        else
        {
            memcpy(&m[mpos * esize], &a[jpos * esize], esize);
            jpos++;
            mpos++;
        }
    }
    memcpy(&a[i * esize], m, esize * ((k - i) + 1));
    free(m);
    return 0;
}

int mgsort(void *data, int size, int esize, int i, int k, int (*compare)(const void *key1, const void *key2))
{
    int j;
    if(i < k)
    {
        /* 计算中间索引值 */
        j = (int)(((i + k - 1)) / 2);
        /* 分别对左右段进行mergesort */
        if(mgsort(data, size, esize, i, j, compare) < 0)
            return -1;
        if(mgsort(data, size, esize, j+1, k, compare) < 0)
            return -1;
        /* 排序后进行融合 */
        if(merge(data, esize, i, j, k, compare) < 0)
            return -1;
    }
    return 0;
}
