#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "graph.h"
#include "list.h"
#include "set.h"


//match 顶点匹配函数 匹配返回1,否则返回0
//destroy 顶点销毁函数

void graph_init(Graph *graph, int (*match)(const void *key1, const void *key2), void(*destroy)(void *data))
{
    graph->vcount = 0;
    graph->ecount = 0;
    graph->match = match;
    graph->destroy = destroy;
    list_init(&graph->adjlists, NULL);
    return;


}


void graph_destroy(Graph *graph)
{
    AdjList *adjlist;
    while(list_size(&graph->adjlists) > 0)
    {
        /* 删除邻接链表中每个顶点的表节点 */
        if(list_rem_next(&graph->adjlists, NULL, (void **)&adjlist) == 0)
        {
            /* 删除每个顶点的set */
            set_destroy(&adjlist->adjacent);
            /* 调用顶点的销毁函数 */
            if(graph->destroy != NULL)
                /* destroy函数用于销毁顶点 */
                graph->destroy(adjlist->vertex);
            /* 清除adjlist指向的对象 */
            free(adjlist);
        }
    }
    list_destroy(&graph->adjlists);
    memset(graph, 0, sizeof(Graph));
    return;

}

int graph_ins_vertex(Graph *graph, const void *data)
{
    ListElmt *element;
    AdjList *adjlist;
    int retval;
    for(element = list_head(&graph->adjlists); element != NULL; element = list_next(element))
    {
        /* 如果图中存在该顶点 */
        if(graph->match(data, ((AdjList *)list_data(element))->vertex))
            return 1;
    }
    /* 不存在就新建该顶点 */
    if((adjlist = (AdjList *)malloc(sizeof(AdjList))) == NULL)
        return -1;
    adjlist->vertex = (void *)data;
    /* 初始化该顶点的邻接表 */
    set_init(&adjlist->adjacent, graph->match, NULL);
    /* 将该节点插入到graph的邻接表中 */
    if((retval = list_ins_next(&graph->adjlists, list_tail(&graph->adjlists), adjlist)) != 0)
    {
        return retval;
    }
    graph->vcount++;
    return 0;
}

/* 在插入边之前顶点必须已经存在 */
/* 将顶点加入到邻接表中，图表和邻接表都存着唯一顶点的信息，这种数据组织结构，如果将权重加入到顶点结构中
   很难管理，特别是当某个顶点入度大于1时，权重很容易被修改，邻接表的顶点最好另外定义数据结构。
*/
int graph_ins_edge(Graph *graph, const void *data1, const void *data2)
{
    ListElmt *element;
    int retval;
    for(element = list_head(&graph->adjlists); element != NULL; element = list_next(element))
    {
        if(graph->match(data2, ((AdjList *)list_data(element))->vertex))
            break;
    }
    if(element == NULL)
        return -1;
    for(element = list_head(&graph->adjlists); element != NULL; element = list_next(element))
    {
        if(graph->match(data1, ((AdjList *)list_data(element))->vertex))
            break;
    }
    if(element == NULL)
        return -1;
    /* 有向图只插单个方向，无向图其实都需要插，不过无非调用两次 */
    /* 该图没有边的权重信息，只是简单的将两个顶点连通了 */
    if((retval = set_insert(&((AdjList *)list_data(element))->adjacent, data2)) != 0)
    {
        return retval;
    }
    graph->ecount++;
    return 0;

}

int graph_rem_vertex(Graph *graph, void **data)
{
    ListElmt *element,
             *temp,
             *prev;
    AdjList *adjlist;
    int found;
    prev = NULL;
    found = 0;
    for(element = list_head(&graph->adjlists); element != NULL; element = list_next(element))
    {
        /* The match argument is a function used by various set operations
         * to determine if two members  match.
         * It should return 1 if key1 is equal to key2, and 0 otherwise.
         * */
        /* 如果顶点在其他的邻接set中，则不能执行删除顶点动作 */
        if(set_is_member(&((AdjList *)list_data(element))->adjacent, *data))
            return -1;
        if(graph->match(*data, ((AdjList *)list_data(element))->vertex))
        {
            temp = element;
            found = 1;
        }
        if(!found)
           /* 维护一个指向前一个节点的指针 */
            prev = element;
    }
    if(!found)
        return -1;
    /* 如果它的邻接表不为空，也不能删除 */
    if(set_size(&((AdjList *)list_data(temp))->adjacent) > 0)
        return -1;
    if(list_rem_next(&graph->adjlists, prev, (void **)&adjlist) != 0)
        return -1;
    *data = adjlist->vertex;
    free(adjlist);
    graph->vcount--;
    return 0;
}

 /* the call removes the vertex specified by data2 from
  *  the adjacency list of data1.
  */
int graph_rem_edge(Graph *graph, void *data1, void **data2)
{
    ListElmt *element;
    for(element = list_head(&graph->adjlists); element != NULL; element = list_next(element))
    {
        if(graph->match(data1, ((AdjList *)list_data(element))->vertex))
            break;
    }
    if(element == NULL)
        return -1;
    if(set_remove(&((AdjList *)list_data(element))->adjacent, data2) != 0)
        return -1;
    graph->ecount--;
    return 0;
}

/* 获得顶点对应的图节点的信息 */
int graph_adjlist(const Graph *graph, const void *data, AdjList **adjlist)
{
    ListElmt *element,
             *prev;
    prev = NULL;
    for(element = list_head(&graph->adjlists); element != NULL; element = list_next(element))
    {
        if(graph->match(data, ((AdjList *)list_data(element))->vertex))
            break;
        prev = element;
    }
    if(element == NULL)
        return -1;
    *adjlist = list_data(element);
    return 0;
}

/* 判断data2顶点是否在data1的邻接表中 */
int graph_is_adjacent(const Graph *graph, const void *data1, const void *data2)
{
    ListElmt *element,
             *prev;
    prev = NULL;
    for(element = list_head(&graph->adjlists); element != NULL; element = list_next(element))
    {
        if(graph->match(data1, ((AdjList *)list_data(element))->vertex))
            break;
        prev = element;
    }
    if(element == NULL)
        return 0;
    return set_is_member(&((AdjList *)list_data(element))->adjacent, data2);
}





