#include <stdio.h>
#include <stdlib.h>

#include "heap.h"

int a[10] = {1,0,4,3,2,9,6,12,7,10};

/* data为动态分配的数据 */
void destroy(void *data)
{
    printf("destroy data: %d ...\n", *((int *)data));
    free(data);
}
int compare(const void *key1, const void *key2)
{
    if(*((int *)key1) > *((int *)key2))
        return 1;
    else if(*((int *)key1) < *((int *)key2))
        return -1;
    else
        return 0;
}

int main()
{
    Heap *heap = (Heap *)malloc(sizeof(Heap));
    heap_init(heap, compare, destroy);
    int *save;


    int i;
    for(i = 0; i < 10; i++)
    {
        int *new = (int *)malloc(sizeof(int));
        new = a+i;
        heap_insert(heap, new);
    }
    printf("Heap size: %d\n", heap_size(heap));
    for(i = 0; i < 10; i++)
    {
        /*
         * extract只能从heap的tree中删除void *的指针，
         * 没有销毁数据，数据存储在save指向的内存中
         */
        heap_extract(heap, (void **)&save);
        printf("extract data: %d\n", *save);
    }
    for(i = 0; i < 10; i++)
    {
        int *new = (int *)malloc(sizeof(int));
        /* copy数据 */
        *new = a[i];
        heap_insert(heap, new);
    }
    heap_destroy(heap);
    return 0;
}
