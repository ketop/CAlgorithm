#ifndef GRAPH_H
#define GRAPH_H

#include <stdlib.h>
#include "list.h"
#include "set.h"

typedef struct AdjList_{
    void *vertex;//顶点表示
    Set  adjacent;//邻接点
}AdjList;
/* 在使用图之前，应该自己定义vertex的类型，主要为顶点的表示方法，及配合搜索算法的标志 */

typedef struct Graph_{
    int vcount;
    int ecount;
    int (*match)(const void *key1, const void *key2);
    void (*destroy)(void *data);
    List adjlists;
}Graph;


typedef enum VertexColor_ {white, gray, black} VertexColor;



void graph_init(Graph *graph, int (*match)(const void *key1, const void *key2), void(*destroy)(void *data));

void graph_destroy(Graph *graph);
/* 0 if inserting the vertex is successful,
 * 1 if the vertex already exists,
 * –1 otherwise.
 */
int graph_ins_vertex(Graph *graph, const void *data);

/* 0 if inserting the vertex is successful,
 * 1 if the vertex already exists,
 * –1 otherwise.
 */
int graph_ins_edge(Graph *graph, const void *data1, const void *data2);

/* 0 if removing the vertex is successful, or –1 otherwise. */
int graph_rem_vertex(Graph *graph, void **data);

/* 0 if removing the vertex is successful, or –1 otherwise. */
int graph_rem_edge(Graph *graph, void *data1, void **data2);

/* 0 if retrieving the adjacency list is successful, or –1 otherwise. */
int graph_adjlist(const Graph *graph, const void *data, AdjList **adjlist);

/* 1 if the second vertex is adjacent to the first vertex, or 0 otherwise. */
int graph_is_adjacent(const Graph *graph, const void *data1, const void *data2);

#define graph_adjlists(graph)  ((graph)->adjlists)
#define graph_vcount(graph)  ((graph)->vcount)
#define graph_ecount(graph) ((graph)->ecount)
#endif
