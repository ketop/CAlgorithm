#include <string.h>
#include "bits.h"

/*
 * 作用对象，bit流，例如: 0010100101011100
 * 索引仍然是以0开始
 * C中的最小指针对象就是char型，因此作用的bits为unsigned char
 * */
int bit_get(const unsigned char *bits, int pos)
{
    unsigned char mask;
    int i;
    mask = 0x80;
    /* mask最多只能向右移7次 */
    /* i [0:6] */
    for(i = 0; i < (pos % 8); i++)
    {
        mask = mask >> 1;
    }
    /* 将bits所取到的字节与mask进行按位与 */
    return (((mask & bits[(int)(pos/8)]) == mask) ? 1 : 0);
}

/* 将bits索引为pos的位设置为state，state为0或1 */
void bit_set(unsigned char *bits, int pos, int state)
{
    unsigned char mask;
    int i;
    mask = 0x80;
    for(i = 0; i < (pos % 8); i++)
    {
        mask = mask >> 1;
    }
    if(state)
        bits[pos / 8] = bits[pos / 8] | mask;
    else
        bits[pos / 8] = bits[pos / 8] & (~mask);
    return;

}

/* 按位异或 */
void bit_xor(const unsigned char *bits1, const unsigned char *bits2, unsigned char *bitsx, int size)
{
    int i;
    for(i = 0; i < size; i++)
    {
        if(bit_get(bits1, i) != bit_get(bits2, i))
            bit_set(bitsx, i, 1);
        else
            bit_set(bitsx, i, 0);
    }
    return;

}

/* 将整个bit流向循环移动count位 */
void bit_rot_left(unsigned char *bits, int size, int count)
{
    int fbit,//记录bits最左侧的一位
        lbit,//记录bits每个字节中最左侧的一位
        i,// 每个字节的移位索引
        j;//count的索引

    if(size > 0)
    {
        for(j = 0; j < count; j++)
        {
            for(i = 0; i <= ((size - 1) / 8); i++)
            {
                /* 取每个字节最左边的bit值 */
                lbit = bit_get(&bits[i], 0);
                /* 如果是最左的bit则记录在fbit中 */
                if(i == 0)
                {
                    /* 对于第一个字节，存入fbit */
                    fbit = lbit;
                }
                /* 如果是其他字节，则移动到下一个字节的最右侧 */
                else
                {
                    /* 对于之后的字节，将前一个字节的第7位置成该字节的第一位 */
                    bit_set(&bits[i - 1], 7, lbit);
                }
                /* 将该字节向左移动一位 */
                bits[i] = bits[i] << 1;
            }
            bit_set(bits, size-1, fbit);
        }
    }
    return;
}
