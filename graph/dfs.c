#include <stdlib.h>
#include <stdio.h>

#include "dfs.h"
#include "graph.h"
#include "list.h"

/* 对某个顶点的邻接链表进行的dfs访问 */
static int dfs_main(Graph *graph, AdjList *adjlist, List *ordered)
{
    AdjList *clr_adjlist;
    DfsVertex *clr_vertex,
              *adj_vertex;
    ListElmt *member;

    /* 首先将该顶点变为灰色 */
    ((DfsVertex *)adjlist->vertex)->color = gray;
    /* 遍历邻接链表 */
    for(member = list_head(&adjlist->adjacent); member != NULL; member = list_next(member))
    {
        /* 获得邻接顶点 */
        adj_vertex = list_data(member);
        /* 获得该顶点的图节点 */
        if(graph_adjlist(graph, adj_vertex, &clr_adjlist) != 0)
            return -1;
        /* 获得该顶点的顶点信息 */
        clr_vertex = clr_adjlist->vertex;
        /* 递归条件：颜色为白色 */
        if(clr_vertex->color == white)
        {
            if(dfs_main(graph, clr_adjlist, ordered) != 0)
                return -1;
        }
    }
    /* 完成该图结点的递归访问 */
    ((DfsVertex *)adjlist->vertex)->color = black;
    /* 插入到order链表中 */
    if(list_ins_next(ordered, NULL, (DfsVertex *)adjlist->vertex) != 0)
        return -1;
    return 0;
}
int dfs(Graph *graph, List *ordered)
{
    DfsVertex *vertex;
    ListElmt *element;
    /* 初始化所有顶点信息 */
    for(element = list_head(&graph_adjlists(graph)); element != NULL; element = list_next(element))
    {
        vertex = ((AdjList *)list_data(element))->vertex;
        vertex->color = white;
    }
    /* 初始化ordered链表 */
    list_init(ordered, NULL);
    for(element = list_head(&graph_adjlists(graph)); element != NULL; element = list_next(element))
    {
        /* 获取一个顶点 */
        vertex = ((AdjList *)list_data(element))->vertex;
        /* 如果顶点为白色，则进行dfs的递归访问 */
        if(vertex->color == white)
        {
            if(dfs_main(graph, (AdjList *)list_data(element), ordered) != 0)
            {
                list_destroy(ordered);
                return -1;
            }
        }
    }

    return 0;
}


