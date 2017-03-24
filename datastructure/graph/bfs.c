#include <stdlib.h>
#include <stdio.h>

#include "bfs.h"
#include "graph.h"
#include "list.h"
#include "queue.h"


/* 广度优先算法 */
/*在图中，从start节点开始访问，将能够访问到的节点存入hops链表中,
 *至于到特定节点的访问路径则无法详细给出
 *广度优先算法需要每个顶点存储BfsVertex信息,所以建立图形顶点时，
 * void * data是由BfsVertex *转换而来的
 */
int bfs(Graph *graph, BfsVertex *start, List *hops)
{
    Queue queue;
    AdjList *adjlist,
            *clr_adjlist;
    BfsVertex *clr_vertex,
              *adj_vertex;
    ListElmt *element,
             *member;
    /* 初始化所有顶点 */
    for(element = list_head(&graph_adjlists(graph)); element != NULL; element = list_next(element))
    {
        /*
         当前的节点存入clr_vertex
         clr_vertex的类型为BfsVertex *
         而vertex为void *
         */
        clr_vertex = ((AdjList *)list_data(element))->vertex;
        /* 如果该节点就是start节点，将其颜色变为灰色，hops=0 */
        if(graph->match(clr_vertex, start))
        {
            clr_vertex->color = gray;
            clr_vertex->hops = 0;
        }
        else
        {
            clr_vertex->color = white;
            clr_vertex->hops = -1;
        }
    }
    /* 初始化队列 */
    queue_init(&queue, NULL);
    /* 获得start顶点的信息，包含其邻接表 */
    if(graph_adjlist(graph, start, &clr_adjlist) != 0)
    {
        queue_destroy(&queue);
        return -1;
    }
    /* 将start定点的信息存入队列 */
    if(queue_enqueue(&queue, clr_adjlist) != 0)
    {
        queue_destroy(&queue);
        return -1;
    }
    /* 循环条件:队列不为空 */
    while(queue_size(&queue) > 0)
    {
        /* 获得队列首部元素，并没有从队列中删除该元素 */
        adjlist = (AdjList *)queue_peek(&queue);
        /* 遍历该顶点的邻接顶点 */
        for(member = list_head(&adjlist->adjacent); member != NULL; member = list_next(member))
        {
            adj_vertex = list_data(member);
            /* 获得该邻接定点的信息 */
            if(graph_adjlist(graph, adj_vertex, &clr_adjlist) != 0)
            {
                /* 如果失败，则销毁queue，程序结束 */
                queue_destroy(&queue);
                return -1;
            }
            /* 对该节点进行判断，如果没有访问过，则涂灰，表示即将访问 */
            clr_vertex = clr_adjlist->vertex;
            if(clr_vertex->color == white)
            {
                clr_vertex->color = gray;
                /* 标明该顶点位于第几层 */
                clr_vertex->hops = ((BfsVertex *)adjlist->vertex)->hops + 1;
                if(queue_enqueue(&queue, clr_adjlist) != 0)
                {
                    queue_destroy(&queue);
                    return -1;
                }
            }
        }

        /* 所有待访问的节点已全部处于队列中，start节点出队列，结束start节点的访问 */
        if(queue_dequeue(&queue, (void **)&adjlist) == 0)
        {
            ((BfsVertex *)adjlist->vertex)->color = black;
        }
        else
        {
            queue_destroy(&queue);
            return -1;
        }

    }
    /* 循环结束，表明所有顶点均已遍历完成 */
    queue_destroy(&queue);
    /* 初始化hops 队列*/
    list_init(hops, NULL);
    /* 获得图的邻接表 */
    for(element = list_head(&graph_adjlists(graph)); element != NULL; element = list_next(element))
    {
        /* 遍历所有顶点，将所有能够访问到的顶点全部存入hops链表中 */
        clr_vertex = ((AdjList *)list_data(element))->vertex;
        if(clr_vertex->hops != -1)
        {
            if(list_ins_next(hops, list_tail(hops), clr_vertex) != 0)
            {
                list_destroy(hops);
                return -1;
            }
        }
    }
    return 0;
}
