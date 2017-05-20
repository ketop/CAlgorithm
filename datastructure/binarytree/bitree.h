#ifndef BITREE_H
#define BITREE_H

//树节点的结构体
typedef struct BiTreeNode_ {
    void *data;
    struct BiTreeNode_ *left;
    struct BiTreeNode_ *right;
}BiTreeNode;

//树的结构体
typedef struct BiTree_ {
    int size;//每一个数据结构，特别是链表类的，定义size可以知道当前的元素总数
    int (*compare)(const void *key1, const void *key2);//为二叉搜索树作准备的成员函数
    void (*destroy)(void *data);//如果数据是动态申请的，则需要定义destroy函数来释放
    BiTreeNode *root;//数的入口，即根节点
}BiTree;


void bitree_init(BiTree *tree, void (*destroy)(void *data));

void bitree_destroy(BiTree *tree);

int bitree_ins_left(BiTree *tree, BiTreeNode *node, const void *data);

int bitree_ins_right(BiTree *tree, BiTreeNode *node, const void *data);

void bitree_rem_left(BiTree *tree, BiTreeNode *node);

void bitree_rem_right(BiTree *tree, BiTreeNode *node);

int bitree_merge(BiTree *merge, BiTree *left, BiTree *right, const void *data);

#define bitree_size(tree)  ((tree)->size)
#define bitree_root(tree)  ((tree)->root)
#define bitree_is_eob(node)  ((node)==NULL)
#define bitree_is_leaf(node)  ((node)->left == NULL && (node)->right == NULL)
#define bitree_data(node)  ((node)->data)
#define bitree_left(node)  ((node)->left)
#define bitree_right(node)  ((node)->right)


#endif
