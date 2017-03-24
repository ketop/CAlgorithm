#include <float.h>
#include <stdlib.h>
#include <stdio.h>

#include "graph.h"
#include "graphalg.h"
#include "list.h"
/*
 * minimum span tree
 * args:
 *      graph : 图的指针
 *      start : 起始顶点
 *      span  : 存储最小生成树的List
 *      match : 顶点之间的匹配
 */
int mst(Graph *graph, const MstVertex *start, List *span, int (*match)(const void *key1, const void *key2))
{
    AdjList *adjlist;
    MstVertex *mst_vertex, //图中的顶点
              *adj_vertex; // 邻接表中的顶点
    ListElmt *element,     //graph 中 adjlist的迭代器
             *member;      //邻接表中的迭代器
    double minimum;
    int found,
        i;
    found = 0;
    /*
     * 初始化图中各顶点;
     * 定位到起点,执行对应的初始化;
     */

    for(element = list_head(&graph_adjlists(graph)); element != NULL; element = list_next(element))
    {
        /*
         *每个MstVertex结构的权重成员不填充，因为它只需要在邻接表中存储权重。
         * */
        mst_vertex = ((AdjList *)list_data(element))->vertex;
        if(match(mst_vertex, start))
        {
            mst_vertex->color = white;
            mst_vertex->key = 0;
            mst_vertex->parent = NULL;
            found = 1;
        }
        else
        {
            mst_vertex->color = white;
            mst_vertex->key = DBL_MAX;
            mst_vertex->parent = NULL;
        }
    }
    if(!found)
        return -1;

    i = 0;
    while(i < graph_vcount(graph))
    {
        minimum = DBL_MAX;
        for(element = list_head(&graph_adjlists(graph)); element != NULL; element = list_next(element))
        {
            mst_vertex = ((AdjList *)list_data(element))->vertex;
            if(mst_vertex->color == white && mst_vertex->key < minimum)
            {
                /* 第一次将定位到起点 */
                minimum = mst_vertex->key;
                adjlist = list_data(element);
            }
        }

        /* 标记起点，表示已访问 */
        ((MstVertex *)adjlist->vertex)->color = black;
        /* 进入到顶点的邻接表 */
        for(member = list_head(&adjlist->adjacent); member != NULL; member = list_next(member))
        {
            /* 获得邻接点 */
            adj_vertex = list_data(member);

            for(element = list_head(&graph_adjlists(graph)); element != NULL; element = list_next(element))
            {
                /* 找到adj_vertex的顶点信息 */
                mst_vertex = ((AdjList *)list_data(element))->vertex;
                if(match(mst_vertex, adj_vertex))
                {
                    /* 如果找到了，并且满足条件:该邻接顶点未曾访问过；从起点到达邻接点的权值 小于 该邻接点的key值 */
                    if(mst_vertex->color == white && adj_vertex->weight < mst_vertex->key)
                    {
                        mst_vertex->key = adj_vertex->weight;
                        mst_vertex->parent = adjlist->vertex;
                    }
                    break;
                }
            }
        }
        i++;
    }
    /* 初始化span链表 */
    list_init(span, NULL);

    for(element = list_head(&graph_adjlists(graph)); element != NULL; element = list_next(element))
    {
        mst_vertex = ((AdjList *)list_data(element))->vertex;
        if(mst_vertex->color == black)
        {
            if(list_ins_next(span, list_tail(span), mst_vertex) != 0)
            {
                list_destroy(span);
                return -1;
            }
        }
    }
    return 0;
}
