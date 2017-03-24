#include <stdlib.h>
#include <string.h>
#include "sort.h"

int bisearch(void *sorted, void *target, int size, int esize, int (*compare)(const void *key1, const void *key2))
{
    int left,
        middle,
        right;
    left = 0;
    right = size - 1;
    while(left <= right)
    {
        middle = (left + right) / 2;
        /* mind the (char *) usage: chage to byte unit */
        switch(compare(((char *)sorted + (esize * middle)), target))
        {
            case -1:
                left = middle + 1;
                break;
            case 0:
                return middle;
            case 1:
                right = middle - 1;
                break;
        }
    }
    return -1;

}
