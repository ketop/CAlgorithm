#include <stdio.h>
#include "sort.h"
int compare(const void *key1, const void *key2)
{
    if(*(int *)key1 > *(int *)key2)
        return 1;
    else if(*(int *)key1 < *(int *)key2)
        return -1;
    else
        return 0;

}

int main()
{
    int a[10] = {7,8,2,6,7,4,5,2,1,0};
    /* issort(a, sizeof(a)/sizeof(int), sizeof(int), compare); */
    /* mgsort(a, 10, sizeof(int), 0, 9, compare ); */
    busort(a, 10, sizeof(int), compare);
    int i;
    for(i = 0; i < 10; ++i)
    {
        printf("%d ", a[i]);
    }
    printf("\n");
    /* int target = 4; */
    /* int index; */
    /* index = bisearch(a, &target, 10, sizeof(int), compare); */
    /* printf("Target lies by indexed: %d", index); */



    return 0;
}
