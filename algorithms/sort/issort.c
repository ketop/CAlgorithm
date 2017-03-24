#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sort.h"

/* Complexity:O(n(n+1)/2 - n) = O(n^2)
 * extra space:  one esize
 *
 *
 * */
/* 插入排序，只需要一个额外的元素大小空间即可进行排序 */
int issort(void *data, int size, int esize, int (*compare)(const void *key1, const void *key2))
{
    char *a = data;
    void *key;
    int i,
        j;
    int flag = 0;//如果紧邻j的一个元素都比j小，则无需额外步骤memcpy
    /* 创建一个元素空间 */
    if((key = (char *)malloc(esize)) == NULL)
        return -1;
    /* j从1开始，因为需要不断与之前的元素进行比较 */
    for(j = 1; j < size; j++)
    {
        /* 从索引为1的元素开始，作为键值与之前的元素进行比较 */
        memcpy(key, &a[j*esize], esize);
        /* 从j-1 到 0 */
        i = j - 1;
        /* while循环能保障j之前的所有元素均已有序，除非key值比左侧所有数都小,因此要移j或0个位置 */
        while( i >= 0 && compare(&a[i*esize], key) > 0 )
        {
            /* 将元素后移一个位置 */
            memcpy(&a[(i+1)*esize], &a[i*esize],esize);
            i--;
            flag = 1;
        }

        if(flag == 1)
        {
            /* 此处不加flag也无大碍，因为key值不会改变，而且下一轮比较就要开始了 */
            memcpy(&a[(i+1)*esize], key, esize);
        }
    }
    free(key);
    return 0;
}
