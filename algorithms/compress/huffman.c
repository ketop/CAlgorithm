#include <limits.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "bits.h"
#include "bitree.h"
#include "compress.h"
#include "pqueue.h"


/* 比较树的freq */
static int compare_freq(const void *tree1, const void *tree2)
{
    HuffNode *root1,
             *root2;
    root1 = (HuffNode *)bitree_data(bitree_root((const BiTree *)tree1));
    root2 = (HuffNode *)bitree_data(bitree_root((const BiTree *)tree2));

    if(root1->freq < root2->freq)
        return 1;
    else if(root1->freq > root2->freq)
        return -1;
    else
        return 0;
}


/* 销毁树 */
static void destroy_tree(void *tree)
{
    bitree_destroy(tree);
    free(tree);
    return;
}

/* 建立huffman tree */
static int build_tree(int *freqs, BiTree **tree)
{
    BiTree *init,
           *merge,
           *left,
           *right;
    /* 优先队列 */
    PQueue pqueue;
    /* 节点数据 */
    HuffNode *data;
    int size,
        c;
    *tree = NULL;
    /* 频率小的树节点优先级高 */
    pqueue_init(&pqueue, compare_freq, destroy_tree);

    for(c = 0; c <= UCHAR_MAX; c++)
    {
        if(freqs[c] != 0)
        {
            /* 创建一棵树 */
            if((init = (BiTree *) malloc(sizeof(BiTree))) == NULL)
            {
                pqueue_destroy(&pqueue);
                return -1;
            }
            data->symbol = c;
            data->freq = freqs[c];
            /* 将数据存入树中 */
            if(bitree_ins_left(init, NULL, data) != 0)
            {
                free(data);
                bitree_destroy(init);
                free(init);
                pqueue_destroy(&pqueue);
                return -1;
            }

            /* 将树存入优先队列 */
            if(pqueue_insert(&pqueue, init) != 0)
            {
                bitree_destroy(init);
                free(init);
                pqueue_destroy(&pqueue);
                return -1;
            }
        }
    }

    /* 保存优先队列中的元素大小 */
    size = pqueue_size(&pqueue);
    for(c = 1; c <= size - 1; c++)
    {
        /* 另一棵树待融合 */
        if((merge = (BiTree *)malloc(sizeof(BiTree))) == NULL)
        {
            pqueue_destroy(&pqueue);
            return -1;
        }

        /* 从优先队列弹出一棵树，使用left指向它 */
        if(pqueue_extract(&pqueue, (void **)&left) != 0)
        {
            pqueue_destroy(&pqueue);
            free(merge);
            return -1;
        }
        /* 从优先队列弹出一棵树，使用right指向它 */
        if(pqueue_extract(&pqueue, (void **)&right) != 0)
        {
            pqueue_destroy(&pqueue);
            free(merge);
            return -1;
        }

        /* 创建一个huffnode结点 */
        if((data = (HuffNode *)malloc(sizeof(HuffNode))) == NULL)
        {
            pqueue_destroy(&pqueue);
            free(merge);
            return -1;
        }


        data->freq = ((HuffNode *)bitree_data(bitree_root(left)))->freq + ((HuffNode *)bitree_data(bitree_root(right)))->freq;
        if(bitree_merge(merge, left, right, data) != 0)
        {
            pqueue_destroy(&pqueue);
            free(merge);
            return -1;
        }
        /* 将merge再次存入优先队列 */
        if(pqueue_insert(&pqueue, merge) != 0)
        {
            pqueue_destroy(&pqueue);
            bitree_destroy(merge);
            free(merge);
            return -1;
        }

        free(left);
        free(right);
    }

    /* 最后大树就长出来了 */
    if(pqueue_extract(&pqueue, (void **)tree) != 0)
    {
        pqueue_destroy(&pqueue);
        return -1;
    }
    else
    {
        pqueue_destroy(&pqueue);
    }
    return 0;
}
/* 建立表 给每个字符编码, 并产生表格*/
static void build_table(BiTreeNode *node, unsigned short code, unsigned char size, HuffCode *table)
{
    /* 若不是空节点 */
    if(!bitree_is_eob(node))
    {
        /* 如果左边不是空节点 */
        if(!bitree_is_eob(bitree_left(node)))
        {
            build_table(bitree_left(node), code << 1, size+1, table);
        }
        /* 如果右边不是空节点 */
        if(!bitree_is_eob(bitree_right(node)))
        {
            build_table(bitree_right(node), (code << 1) | 0x0001, size+1, table);
        }
        /* 如果左右都为空节点,此时找到字符节点 */
        if(bitree_is_eob(bitree_left(node))&&bitree_is_eob(bitree_right(node)))
        {
            /* converts the unsigned short integer hostshort from host byte order to network byte order. */
            /* 递归编码顺序是从底部向上的，因此需要调换顺序 */
            code = htons(code);
            table[((HuffNode *)bitree_data(node))->symbol].used = 1;
            table[((HuffNode *)bitree_data(node))->symbol].code = code;
            table[((HuffNode *)bitree_data(node))->symbol].size = size;
        }
    }
    return;
}

/* 压缩函数 */
int huffman_compress(const unsigned char *original, unsigned char **compressed, int size)
{
    BiTree *tree;
    HuffCode table[UCHAR_MAX+1];
    int freqs[UCHAR_MAX+1],
        max,
        scale,
        hsize,
        ipos,
        opos,
        cpos,
        c,
        i;
    unsigned char *comp,
                  *temp;
    *compressed = NULL;
    /* 初始化freqs数组 */
    for(c = 0; c <= UCHAR_MAX; c++)
    {
        freqs[c] = 0;
    }
    ipos = 0;
    if(size > 0)
    {
        /* 统计原始字符original中各字符出现的频率, 数组下标表示字符 */
        while(ipos < size)
        {
            freqs[original[ipos]]++;
            ipos++;
        }
    }
    max = UCHAR_MAX;
    /*
     *  Huffman code: symbol出现越频繁，其code编码位数越小，主要原因是其数的组织结构
     *  UCHAR_MAX = 255 因为扩展的ascii表编码的10进制可以达到255
     *
     *
     *
     *
     * */
    for(c = 0; c <= UCHAR_MAX; c++)
    {
        /* max中存储出现频率最大的字符的频率 */
        if(freqs[c] > max)
            max = freqs[c];
    }
    /* scale */
    for(c = 0; c <= UCHAR_MAX; c++)
    {
        /* 将频率按比例缩小到UCHAR_MAX能够表示的范围之内 */
        /* 频率主要用来构建树的时候进行节点排序，所以按理说缩小一定比率是可行的，
         * 编码无论如何也是唯一的，与频率无关.
         * */
        scale = (int)(freqs[c] / ((double)max / (double)UCHAR_MAX));
        if(scale == 0 && freqs[c] != 0)
            freqs[c] = 1;
        else
            freqs[c] = scale;
    }

    /* 根据频率创建huffman tree */
    if(build_tree(freqs, &tree) != 0)
        return -1;

    /* 初始化table */
    for(c = 0; c <= UCHAR_MAX; c++)
        memset(&table[c], 0, sizeof(HuffCode));

    /* 创建table表 */
    build_table(bitree_root(tree), 0x0000, 0, table);
    bitree_destroy(tree);
    free(tree);

    hsize = sizeof(int) + (UCHAR_MAX + 1);

    if((comp = (unsigned char *)malloc(hsize)) == NULL)
        return -1;
    /* 现将size放入comp中,表明原来的字符文件有多少个symbol */
    memcpy(comp, &size, sizeof(int));

    /* comp之后的字节记录每个symbol的频率(scaled) */
    for(c = 0; c <= UCHAR_MAX; c++)
        comp[sizeof(int) + c] = (unsigned char)freqs[c];

    /* original下标索引 */
    ipos = 0;

    /* 位指针 */
    opos = hsize * 8;
    while(ipos < size)
    {
        c = original[ipos];
        /* 每个symbol的code的位数为table[i].size */
        for(i = 0; i < table[c].size; i++)
        {
            /* 如果满字节，则重新分配空间 */
            if(opos % 8 == 0)
            {
                if((temp = (unsigned char *)realloc(comp, (opos / 8) + 1)) == NULL)
                {
                    free(comp);
                    return -1;
                }
                comp = temp;
            }

            /* 16 bits code的下标索引 */
            cpos = (sizeof(short) * 8) - table[c].size + i;
            bit_set(comp, opos, bit_get((unsigned char *)&table[c].code, cpos));
            opos++;
        }
        /* 下一个symbol */
        ipos++;
    }
    *compressed = comp;
    return ((opos - 1) / 8) + 1;
}

/* 解压缩 */
int huffman_uncompress(const unsigned char *compressed, unsigned char **original)
{
    BiTree *tree;
    BiTreeNode *node;

    int freqs[UCHAR_MAX + 1],
        hsize,
        size,
        ipos,
        opos,
        state,
        c;
    unsigned char *orig,
                  *temp;
    original = NULL;
    orig = NULL;

    /* 提取原始字符串的总的symbol数 */
    hsize = sizeof(int) + (UCHAR_MAX + 1);
    memcpy(&size, compressed, sizeof(int));

    /* 提取字符的频率分布表 */
    for(c = 0; c <= UCHAR_MAX; c++)
        freqs[c] = compressed[sizeof(int) + c];

    /* 重新生成huffman tree */
    if(build_tree(freqs, &tree) != 0)
        return -1;
    /* 指向压缩部分内容起始地方 */
    ipos = hsize * 8;

    opos = 0;
    /* 开始译码了 */
    node = bitree_root(tree);
    while(opos < size)
    {
        /* 获得bit位state */
        state = bit_get(compressed, ipos);
        ipos++;
        /* 为0取左子树 */
        if(state == 0)
        {
            /* 如果树为空，且没有左节点 */
            if(bitree_is_eob(node) || bitree_is_eob(bitree_left(node)))
            {
                bitree_destroy(tree);
                free(tree);
                return -1;
            }
            else
                node = bitree_left(node);
        }
        else
        /* 为1取右子树 */
        {
            if(bitree_is_eob(node) || bitree_is_eob(bitree_right(node)))
            {
                bitree_destroy(tree);
                free(tree);
                return -1;
            }
            else
                node = bitree_right(node);
        }
        /* 到达编码叶子节点 */
        if(bitree_is_eob(bitree_left(node)) && bitree_is_eob(bitree_right(node)))
        {
            if(opos > 0)
            {
                /*动态调整temp大小*/
                if((temp = (unsigned char *)realloc(orig, opos+1)) == NULL)
                {
                    bitree_destroy(tree);
                    free(tree);
                    free(orig);
                    return -1;
                }
                orig = temp;
            }
            else
            {
                /* 创建temp存储字符 */
                if((orig = (unsigned char *)malloc(1)) == NULL)
                {
                    bitree_destroy(tree);
                    free(tree);
                    return -1;
                }
            }
            orig[opos] = ((HuffNode *)bitree_data(node))->symbol;
            opos++;
            /* 下一个symbol译码 */
            node = bitree_root(tree);
        }
    }
    bitree_destroy(tree);
    free(tree);
    *original = orig;
    return opos;
}
