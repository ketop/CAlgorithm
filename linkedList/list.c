#include <stdio.h>
#include <stdlib.h>
#include "list.h"



/* initialize a list
arg:
      arg1:   list to initialize
      arg2:   destroy method to data object, like malloc data from heap
*/
void list_init(List *list, void (*destroy)(void *data))
{
    list->size = 0;
    list->destroy = destroy;
    list->head = NULL;
    list->tail = NULL;
}

/* destroy a list
 arg:
      arg1:list to destroy
*/
void list_destroy(List *list)
{
    void *data;

    while(list_size(list) > 0)
    {
        if(list_rem_next(list, NULL, (void **)&data) == 0 && list->destroy != NULL)
        {
            list->destroy(data);
        }
    }
}

/* insert a new element next to specified element
 arg:
      arg1: list to process
      arg2: specified element
      arg3: data in new element
*/
int list_ins_next(List *list, ListElmt *element, const void *data)
{
    ListElmt *new_element;

    if((new_element = (ListElmt *)malloc(sizeof(ListElmt))) == NULL)
        return -1;

    new_element->data = (void *)data;
    if(element == NULL)
    {
        if(list_size(list) == 0)
            list->tail = new_element;

        new_element->next = list->head;
        list->head = new_element;
    }
    else
    {
        if(element->next == NULL)
        {
            list->tail = new_element;
        }
        new_element = element->next;
        element->next = new_element;
    }
    list->size++;
    return 0;
}

/* remove a element next to specified element
 arg:
      arg1: list to process
      arg2: specified element
      arg3: data in element to be removed
*/
int list_rem_next(List *list, ListElmt *element, void **data)
{
    ListElmt *old_element;

    if(list_size(list) == 0)
        return -1;
    if(element == NULL)
    {
        //remove element next to head
        *data = list->head->data;
        old_element = list->head;
        list->head = list->head->next;

        if(list_size(list) == 1)
            list->tail = NULL;
    }
    else
    {
        if(element->next == NULL)
            return -1;
        *data = element->next->data;
        old_element = element->next;
        element->next = element->next->next;

        if(element->next == NULL)
            list->tail = element;
    }
    free(old_element);

    list->size--;
    return 0;
}
