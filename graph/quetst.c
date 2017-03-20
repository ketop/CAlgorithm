#include "queue.h"
#include <stdio.h>
int a[10] = {1, 2, 3, 4, 5, 7, 6, 8, 9, 0};
int main()
{
    Queue queue;
    queue_init(&queue, NULL);
    int i;
    int *save;
    for(i = 0; i < 10; ++i)
    {
        queue_enqueue(&queue, (a + i));
    }
    for(i = 0; i < 10; ++i)
    {
        queue_dequeue(&queue, (void **)&save);
        printf("%d\n",*((int *)save));
    }

    return 0;

}
