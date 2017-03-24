#include "sort.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void swap(void *key1, void *key2, int esize)
{
    char *temp = malloc(esize);
    if(temp == NULL)
    {
        perror("malloc fail");
        return;
    }
    memcpy(temp, key1, esize);
    memcpy(key1, key2, esize);
    memcpy(key2, temp, esize);
}


/* 最坏的情况为逆序排列 */
/* Complexity: O(n(n+1)/2) = O(n^2) */

int busort(void *data, int size, int esize, int (*compare)(const void *key1, const void *key2))
{
    int i, j, r;
    int flag = 0;
    for(r = 0; r < size; ++r)
    {
        for(i = 0; i < size - r; ++i)
        {
            j = i + 1;
            if(compare(data+i*esize, data+j*esize) > 0)
            {
                swap(data+i*esize, data+j*esize, esize);
                flag = 1;
            }
        }
        if(flag == 0)
            break;

    }
}
