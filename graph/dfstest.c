#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "dfs.h"

char subject[][7] = {"CS100", "CS150", "CS200", "CS300", "MA100", "MA200", "MA300"};

int match(const void *key1, const void *key2)
{
    if(((DfsVertex *)key1)->data ==  ((DfsVertex *)key2)->data)
        return 1;
    else
        return 0;
}

void destroy(void *data)
{
    ((DfsVertex *)(data))->data = NULL;
    free((DfsVertex *)(data));

}

int main()
{
    Graph graph;
    List list;
    ListElmt *element;
    graph_init(&graph, match, destroy);
    DfsVertex *vertex1 = malloc(sizeof(DfsVertex));
    vertex1->data = subject[0];
    graph_ins_vertex(&graph, vertex1);
    DfsVertex *vertex2 = malloc(sizeof(DfsVertex));
    vertex2->data = subject[1];
    graph_ins_vertex(&graph, vertex2);
    DfsVertex *vertex3 = malloc(sizeof(DfsVertex));
    vertex3->data = subject[2];
    graph_ins_vertex(&graph, vertex3);
    DfsVertex *vertex4 = malloc(sizeof(DfsVertex));
    vertex4->data = subject[3];
    graph_ins_vertex(&graph, vertex4);
    DfsVertex *vertex5 = malloc(sizeof(DfsVertex));
    vertex5->data = subject[4];
    graph_ins_vertex(&graph, vertex5);
    DfsVertex *vertex6 = malloc(sizeof(DfsVertex));
    vertex6->data = subject[5];
    graph_ins_vertex(&graph, vertex6);
    DfsVertex *vertex7 = malloc(sizeof(DfsVertex));
    vertex7->data = subject[6];
    graph_ins_vertex(&graph, vertex7);

    graph_ins_edge(&graph, vertex1, vertex3);
    graph_ins_edge(&graph, vertex3, vertex4);
    graph_ins_edge(&graph, vertex5, vertex4);
    graph_ins_edge(&graph, vertex5, vertex6);
    graph_ins_edge(&graph, vertex6, vertex7);
    graph_ins_edge(&graph, vertex4, vertex7);

    dfs(&graph, &list);
    for(element = list_head(&list); element != NULL; element = list_next(element))
    {
        char *s;
        s = (char *)(((DfsVertex *)(element->data))->data);
        printf("%s ", s);
    }











    graph_destroy(&graph);
    return 0;
}
