#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "bitree.h"


void bitree_init(BiTree *tree, void (*destroy)(void *data))
{
    tree->size = 0;
    tree->destroy = destroy;
    tree->root = NULL;
    return;

}

void bitree_destroy(BiTree *tree)
{
    bitree_rem_left(tree, NULL);
    memset(tree, 0, sizeof(BiTree));
    return;
}

int bitree_ins_left(BiTree *tree, BiTreeNode *node, const void *data)
{
    BiTreeNode *new_node,
               **position;
    /* 在树中的NULL节点的左侧插入值 */
    if(node == NULL)
    {
        /* 除非树为空 */
        if(bitree_size(tree) > 0)
            return -1;
        position = &tree->root;
    }
    else
    {
        /* 除非node节点左子树为空 */
        if(bitree_left(node) != NULL)
            return -1;
        position = &node->left;
    }
    if((new_node = (BiTreeNode *)malloc(sizeof(BiTreeNode))) == NULL)
        return -1;

    /* 初始化new_node */
    new_node->data = (void *)data;
    new_node->left = NULL;
    new_node->right = NULL;
    *position = new_node;

    tree->size++;
    return 0;
}

int bitree_ins_right(BiTree *tree, BiTreeNode *node, const void *data)
{
    BiTreeNode *new_node,
               **position;

    /* 如果指定的节点为NULL，除非树为空，否则无法插入 */
    if(node == NULL)
    {
        if(bitree_size(tree) > 0)
            return -1;
        position = &tree->root;
    }
    else
    {
        if(bitree_right(node) != NULL)
            return -1;
        position = &node->right;
    }

    if((new_node = (BiTreeNode *)malloc(sizeof(BiTreeNode))) == NULL)
        return -1;
    new_node->data = (void *)data;
    new_node->left = NULL;
    new_node->right = NULL;
    *position = new_node;

    tree->size++;
    return 0;

}
/* 删除以node为父节点的左子树 */
void bitree_rem_left(BiTree *tree, BiTreeNode *node)
{
    BiTreeNode **position;
    /* 树为空就不做任何操作 */
    if(bitree_size(tree) == 0)
        return;
    /* 如果node为NULL，则删除tree中所有节点 */
    if(node == NULL)
        position = &tree->root;
    else
        position = &node->left;
    if(*position != NULL)
    {
        /* 先递归删除子树中的所有元素 */
        bitree_rem_left(tree, *position);
        bitree_rem_right(tree, *position);

        /* 删除存储的数据 */
        if(tree->destroy != NULL)
        {
            tree->destroy((*position)->data);
        }
        /* 删除节点 */
        free(*position);
        *position = NULL;
        tree->size--;
    }
    return;

}

void bitree_rem_right(BiTree *tree, BiTreeNode *node)
{
    BiTreeNode **position;

    if(bitree_size(tree) == 0)
        return;
    if(node == NULL)
        position = &tree->root;
    else
        position = &node->right;
    if(*position != NULL)
    {
        bitree_rem_left(tree, *position);
        bitree_rem_right(tree, *position);
        if(tree->destroy != NULL)
        {
            tree->destroy((*position)->data);
        }
        free(*position);
        *position = NULL;
        tree->size--;
    }
    return;
}

/* 以merge作为root节点，存储data,并以left和right作为其子树 */
int bitree_merge(BiTree *merge, BiTree *left, BiTree *right, const void *data)
{
    bitree_init(merge, left->destroy);

    /* merge为空树，因此调用插入函数 */
    if(bitree_ins_left(merge, NULL, data))
    {
        bitree_destroy(merge);
        return -1;
    }

    bitree_root(merge)->left = bitree_root(left);
    bitree_root(merge)->right = bitree_root(right);

    merge->size = merge->size + bitree_size(left) + bitree_size(right);

    /* 让left和right脱离其根节点的管辖 */
    left->root = NULL;
    left->size = 0;
    right->root = NULL;
    right->size = 0;

    return 0;

}
