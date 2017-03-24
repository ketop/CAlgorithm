#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "bfs.h"

int vertex[5] = {1, 2, 3, 4, 5};
int match(const void *key1, const void *key2)
{
    if(((BfsVertex *)key1)->data == ((BfsVertex *)key2)->data)
        return 1;
    else
        return 0;
}

void destroy(void *data)
{
    free((int *)(((BfsVertex *)data)->data));
    free((BfsVertex *)data);
}

int main()
{
    Graph graph;
    List hops;
    ListElmt *element;
    graph_init(&graph, match, destroy);

    BfsVertex *vertex1 = malloc(sizeof(BfsVertex));
    int *bench1 = malloc(sizeof(int));
    *bench1 = 1;
    vertex1->data = bench1;
    graph_ins_vertex(&graph, vertex1);

    BfsVertex *vertex2 = malloc(sizeof(BfsVertex));
    int *bench2 = malloc(sizeof(int));
    *bench2 = 2;
    vertex2->data = bench2;
    graph_ins_vertex(&graph, vertex2);

    BfsVertex *vertex3 = malloc(sizeof(BfsVertex));
    int *bench3 = malloc(sizeof(int));
    *bench3 = 3;
    vertex3->data = bench3;
    graph_ins_vertex(&graph, vertex3);

    BfsVertex *vertex4 = malloc(sizeof(BfsVertex));
    int *bench4 = malloc(sizeof(int));
    *bench4 = 4;
    vertex4->data = bench4;
    graph_ins_vertex(&graph, vertex4);

    BfsVertex *vertex5 = malloc(sizeof(BfsVertex));
    int *bench5 = malloc(sizeof(int));
    *bench5 = 5;
    vertex5->data = bench5;
    graph_ins_vertex(&graph, vertex5);

    graph_ins_edge(&graph, vertex1, vertex2);
    graph_ins_edge(&graph, vertex1, vertex3);
    graph_ins_edge(&graph, vertex2, vertex3);
    graph_ins_edge(&graph, vertex2, vertex4);
    graph_ins_edge(&graph, vertex3, vertex2);
    graph_ins_edge(&graph, vertex3, vertex3);
    graph_ins_edge(&graph, vertex3, vertex4);
    graph_ins_edge(&graph, vertex4, vertex1);
    graph_ins_edge(&graph, vertex5, vertex3);
    graph_ins_edge(&graph, vertex5, vertex4);

    bfs(&graph, vertex1, &hops);

    int *ta;
    for(element = list_head(&hops); element != 0; element = list_next(element))
    {
        ta = (int *)(((BfsVertex *)list_data(element))->data);
        printf("%d,", *ta);

    }

    graph_destroy(&graph);

    bench1 = NULL;
    bench2 = NULL;
    bench3 = NULL;
    bench4 = NULL;
    bench5 = NULL;


    return 0;
}
