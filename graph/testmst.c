#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "graphalg.h"

int a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};

int match(const void *key1, const void *key2)
{
    if(((MstVertex *)key1)->data == ((MstVertex *)key2)->data)
        return 1;
    else
        return 0;
}

void destroy(void *data)
{
    ((MstVertex *)data)->data = NULL;
    free(data);
}

int main()
{
    Graph graph;
    List span;
    ListElmt *element;
    graph_init(&graph, match, destroy);
    MstVertex *vertex1 = malloc(sizeof(MstVertex));
    vertex1->data = a;
    graph_ins_vertex(&graph, vertex1);
    MstVertex *vertex2 = malloc(sizeof(MstVertex));
    vertex2->data = a+1;
    graph_ins_vertex(&graph, vertex2);
    MstVertex *vertex3 = malloc(sizeof(MstVertex));
    vertex3->data = a+2;
    graph_ins_vertex(&graph, vertex3);
    MstVertex *vertex4 = malloc(sizeof(MstVertex));
    vertex4->data = a+3;
    graph_ins_vertex(&graph, vertex4);
    MstVertex *vertex5 = malloc(sizeof(MstVertex));
    vertex5->data = a+4;
    graph_ins_vertex(&graph, vertex5);
    MstVertex *vertex6 = malloc(sizeof(MstVertex));
    vertex6->data = a+5;
    graph_ins_vertex(&graph, vertex6);
    MstVertex *vertex7 = malloc(sizeof(MstVertex));
    vertex7->data = a+6;
    graph_ins_vertex(&graph, vertex7);
    /* MstVertex *vertex8 = malloc(sizeof(MstVertex)); */
    /* vertex8->data = a+7; */
    /* graph_ins_vertex(&graph, vertex8); */
    /* MstVertex *vertex9 = malloc(sizeof(MstVertex)); */
    /* vertex9->data = a+8; */
    /* graph_ins_vertex(&graph, vertex9); */
    /* MstVertex *vertex10 = malloc(sizeof(MstVertex)); */
    /* vertex10->data = a+9; */
    /* graph_ins_vertex(&graph, vertex10); */
    vertex2->weight = 14;
    graph_ins_edge(&graph, vertex1, vertex2);
    vertex1->weight = 10;
    graph_ins_edge(&graph, vertex2, vertex1);
    vertex6->weight = 25;
    graph_ins_edge(&graph, vertex1, vertex6);
    vertex1->weight = 10;
    graph_ins_edge(&graph, vertex6, vertex1);
    vertex3->weight = 12;
    graph_ins_edge(&graph, vertex2, vertex3);
    vertex2->weight = 14;
    graph_ins_edge(&graph, vertex3, vertex2);
    vertex7->weight = 24;
    graph_ins_edge(&graph, vertex2, vertex7);
    vertex2->weight = 14;
    graph_ins_edge(&graph, vertex7, vertex2);
    vertex4->weight = 22;
    graph_ins_edge(&graph, vertex3, vertex4);
    vertex3->weight = 12;
    graph_ins_edge(&graph, vertex4, vertex3);
    vertex7->weight = 24;
    graph_ins_edge(&graph, vertex4, vertex7);
    vertex4->weight = 22;
    graph_ins_edge(&graph, vertex7, vertex4);
    vertex7->weight = 24;
    graph_ins_edge(&graph, vertex5, vertex7);
    vertex5->weight = 22;
    graph_ins_edge(&graph, vertex7, vertex5);
    vertex6->weight = 25;
    graph_ins_edge(&graph, vertex5, vertex6);
    vertex5->weight = 22;
    graph_ins_edge(&graph, vertex6, vertex5);
    vertex5->weight = 22;
    graph_ins_edge(&graph, vertex4, vertex5);
    vertex4->weight = 22;
    graph_ins_edge(&graph, vertex5, vertex4);

    mst(&graph, vertex1, &span, match);
    int *vertex;
    int *vertparent;
    MstVertex *parent;
    for(element = list_head(&span); element != NULL; element = list_next(element))
    {
        vertex = (int *)(((MstVertex *)list_data(element))->data);
        parent = ((MstVertex *)list_data(element))->parent;
        if(parent == NULL)
        {
            printf("NULL -> %d   ", *vertex);
        }
        else
        {
            vertparent = (int *)(parent->data);
            printf("%d -> %d   ", *vertparent, *vertex );
        }

    }





    graph_destroy(&graph);
    return 0;
}
