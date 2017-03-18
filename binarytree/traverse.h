#include <stdio.h>
#include "list.h"
#include "bitree.h"



/* 所有遍历方法都是递归遍历，只要节点不为空，就按指定的顺序来遍历它及它的左右子节点 */
/* 先对那个节点进行操作，就将该操作放在对应的位置 */
int preoder(const BiTreeNode *node, List *list)
{
    /* 递归条件：节点不为空 */
    if(!bitree_is_eob(node))
    {
        if(list_ins_next(list, list_tail(list), bitree_data(node)) != 0)
            return -1;
        if(!bitree_is_eob(bitree_left(node)))
            if(preoder(bitree_left(node), list) != 0)
                return -1;
        if(!bitree_is_eob(bitree_right(node)))
            if(preoder(bitree_right(node), list) != 0)
                return -1;
    }
    return 0;
}

int inorder(const BiTreeNode *node, List *list)
{
    if(!bitree_is_eob(node))
    {
        if(!bitree_is_eob(bitree_left(node)))
            if(inorder(bitree_left(node), list))
                return -1;
        if(list_ins_next(list, list_tail(list), bitree_data(node)) != 0)
            return -1;
        if(!bitree_is_eob(bitree_right(node)))
            if(inorder(bitree_right(node), list) != 0)
                return -1;
    }
    return 0;
}


int postorder(const BiTreeNode *node, List *list)
{
    if(!bitree_is_eob(node))
    {
        if(!bitree_is_eob(bitree_left(node)))
            if(inorder(bitree_left(node), list))
                return -1;
        if(!bitree_is_eob(bitree_right(node)))
            if(inorder(bitree_right(node), list) != 0)
                return -1;
        if(list_ins_next(list, list_tail(list), bitree_data(node)) != 0)
            return -1;
    }
    return 0;
}
