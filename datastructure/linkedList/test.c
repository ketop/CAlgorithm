#include <stdio.h>

#include "clist.h"

void destroy(void *data)
{
    data = NULL;
}

int main()
{
    CList *list = (CList *)malloc(sizeof(CList));
    clist_init(list, NULL);
    int a[5];
    int i;
    for(i = 0; i < 5; ++i)
    {
        a[i] = 5 - i;
    }
    clist_ins_next(list, clist_head(list), &a[1]);
    clist_ins_next(list, clist_head(list), &a[2]);
    clist_ins_next(list, clist_head(list), &a[4]);
    int size = clist_size(list);
    printf("%d\n", size);
    CListElmt *current = clist_next(list->head);
    while(current != clist_head(list))
    {
        printf("%x===", *(int *)(current->data));
        current = current->next;
    }
    printf("\n");
    int *store;
    clist_rem_next(list, clist_head(list), (void **)&store);
    printf("%d has been removed\n", *store);
    /* current = list->head; */
    /* while(current != NULL) */
    /* { */
    /*     printf("%x===", *(int *)(current->data)); */
    /*     current = current->next; */
    /* } */
    /* printf("\n"); */





    /* int *store; */
    /* quque_dequeue(queue,(void **)&store); */
    /* printf("%d\n", *store); */
    clist_destroy(list);
    return 0;
}
