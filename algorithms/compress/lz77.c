#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#include "bits.h"
#include "compress.h"

static int compare_win(const unsigned char *window, const unsigned char *buffer, int *offset, unsigned char *next)
{
    int match,
        longest,
        i,
        j,
        k;
    *offset = 0;
    longest = 0;
    *next = buffer[0];
    /* 从窗口中依次搜索与buffer相匹配的字符 */
    /* for循环实现最长最合适的匹配 */
    for(k = 0; k < LZ77_WINDOW_SIZE; k++)
    {
        i = k;
        j = 0;
        match = 0;

        while(i < LZ77_WINDOW_SIZE && j < LZ77_BUFFER_SIZE - 1)
        {
            if(window[i] != buffer[j])
                break;
            /* 匹配最大化 */
            match++;
            i++;
            j++;
        }

        /* 如果存在匹配项 */
        if(match > longest)
        {
            /* 记录window偏移量，buffer的偏移量 */
            *offset = k;
            /* 记录此次最大匹配 */
            longest = match;
            /* 记录匹配字符串之后的首个不匹配的字符 */
            *next = buffer[j];
        }
    }

    return longest;
}

int lz77_compress(const unsigned char *original, unsigned char **compressed, int size)
{
    unsigned char window[LZ77_WINDOW_SIZE],/* 窗体长度 */
                  buffer[LZ77_BUFFER_SIZE],/* 前向buffer */
                  *comp,
                  *temp,
                  next;
    int offset,
        length,
        remaining,
        hsize,
        ipos,
        opos,
        tpos,
        i;
    *compressed = NULL;
    /* 开辟压缩头 */
    hsize = sizeof(int);
    if((comp = (unsigned char *)malloc(hsize)) == NULL)
        return -1;
    /* 用comp记录原始长度信息 */
    memcpy(comp, &size, sizeof(int));
    /* 初始化窗口及缓冲区 */
    memset(window, 0, LZ77_WINDOW_SIZE);
    memset(buffer, 0, LZ77_BUFFER_SIZE);

    /* 装载缓冲区 */
    ipos = 0;
    for(i = 0; i < LZ77_BUFFER_SIZE && ipos < size; i++)
    {
        buffer[i] = original[ipos];
        ipos++;
    }

    /* bit位指示索引 */
    opos = hsize * 8;
    /* remaining记录余下未压缩的字符数 */
    remaining = size;
    unsigned long token;
    int tbits;
    while(remaining > 0)
    {
        /* 如果存在匹配 */
        if((length = compare_win(window, buffer, &offset, &next)) != 0)
        {
            /* 装载type位 */
            token = 0x00000001 << (LZ77_PHRASE_BITS - LZ77_TYPE_BITS);
            /* 装载offset位 */
            token = token | (offset << (LZ77_PHRASE_BITS - LZ77_TYPE_BITS - LZ77_WINOFF_BITS));
            /* 装载匹配的长度位 */
            token = token | (length << (LZ77_PHRASE_BITS - LZ77_TYPE_BITS - LZ77_WINOFF_BITS - LZ77_BUFLEN_BITS));
            /* 装载next位 */
            token = token | next;
            tbits = LZ77_PHRASE_BITS;
        }
        else
        {
            token = 0x00000000;
            token = token | next;
            tbits = LZ77_SYNBOL_BITS;
        }
        /* 将token转换为 big endian 存储方式 */
        token = htonl(token);

        for(i = 0; i < tbits; ++i)
        {
            if(opos % 8 == 0)
            {
                /* 满字节则动态调整comp大小 */
                if((temp = (unsigned char *)realloc(comp, (opos / 8) + 1)) == NULL)
                {
                    free(comp);
                    return -1;
                }
                comp = temp;
            }
            /* token的下标索引 */
            tpos = (sizeof(unsigned long) * 8) - tbits + i;

            bit_set(comp, opos, bit_get((unsigned char *)&token, tpos));
            opos++;
        }
        length++;
        /* 滑动窗口 */
        memmove(&window[0], &window[length], LZ77_WINDOW_SIZE - length);
        memmove(&window[LZ77_WINDOW_SIZE - length], &buffer[0], length);

        /* 载入更多的look-ahead buffer */
        memmove(&buffer[0], &buffer[length], LZ77_BUFFER_SIZE - length);
        for( i = LZ77_BUFFER_SIZE - length; i < LZ77_BUFFER_SIZE && ipos < size; i++ )
        {
            buffer[i] = original[ipos];
            /* original指针增加 */
            ipos++;
        }

        remaining = remaining - length;
    }
    *compressed = comp;
    /* 返回当前压缩文件的字节大小 */
    return ((opos - 1) / 8) + 1;

}

int lz77_uncompress(const unsigned char *compressed, unsigned char **original)
{
    unsigned char window[LZ77_WINDOW_SIZE],
                  buffer[LZ77_BUFFER_SIZE],
                  *orig,
                  *temp,
                  next;
    int offset,
        length,
        remaining,
        hsize,
        size,
        ipos,
        opos,
        tpos,
        state,
        i;
    *original = orig = NULL;
    /* header记录的源文件大小 */
    hsize = sizeof(int);
    memcpy(&size, compressed, sizeof(int));

    memset(window, 0, LZ77_WINDOW_SIZE);
    memset(buffer, 0, LZ77_BUFFER_SIZE);

    /* 压缩文件的bit位指针 */
    ipos = hsize * 8;
    /* 已解压的文件的指针 */
    opos = 0;
    remaining = size;

    while(remaining > 0)
    {
        state = bit_get(compressed, ipos);
        ipos++;
        /* 如果为phrase token */
        if(state == 1)
        {

            memset(&offset, 0, sizeof(int));

            for(i = 0; i < LZ77_WINOFF_BITS; i++)
            {
                tpos = (sizeof(int) * 8) - LZ77_WINOFF_BITS + i;
                /* 取得offset */
                bit_set((unsigned char *)&offset, tpos, bit_get(compressed, ipos));
                ipos++;
            }

            memset(&length, 0, sizeof(int));

            for(i = 0; i < LZ77_BUFLEN_BITS; i++)
            {
                tpos = (sizeof(int) * 8) - LZ77_BUFLEN_BITS + i;
                /* 取得length */
                bit_set((unsigned char *)&length, tpos, bit_get(compressed, ipos));
                ipos++;
            }

            next = 0x00;
            for(i = 0; i < LZ77_NEXT_BITS; i++)
            {
                tpos = (sizeof(unsigned char) * 8) - LZ77_NEXT_BITS + i;
                /* 取得下一个字符 */
                bit_set((unsigned char *)&next, tpos, bit_get(compressed, ipos));
                ipos++;
            }

            /* 反转为主机字节序 */
            offset = ntohl(offset);
            length = ntohl(length);

            if(opos > 0)
            {
                if((temp = (unsigned char *)realloc(orig, opos+length+1)) == NULL)
                {
                    free(orig);
                    return -1;
                }

                orig = temp;
            }
            else
            {
                if((orig = (unsigned char *)malloc(length + 1)) == NULL)
                    return -1;
            }

            i = 0;
            /* 将解压的字符装入orig */
            while(i < length && remaining > 0)
            {
                orig[opos] = window[offset + i];
                opos++;

                /* buffer也装载了字符 */
                buffer[i] = window[offset + i];
                i++;

                remaining--;
            }

            if(remaining > 0)
            {
                orig[opos] = next;
                opos++;
                /* buffer装载了首个未匹配的字符 */
                buffer[i] = next;
                remaining--;
            }

            length++;

        }
        /* 如果为symbol token */
        else
        {
            /* 将next清除 */
            next = 0x00;
            for(i = 0; i < LZ77_NEXT_BITS; i++)
            {
                tpos = (sizeof(unsigned char) * 8) - LZ77_NEXT_BITS + i;
                bit_set((unsigned char *)&next, tpos, bit_get(compressed, ipos));
                ipos++;
            }
            if(opos > 0)
            {
                if((temp = (unsigned char *) realloc(orig, opos + 1)) == NULL)
                {
                    free(orig);
                    return -1;
                }
                orig = temp;
            }
            else
            {
                if((orig = (unsigned char *)malloc(1)) == NULL)
                    return -1;
            }

            orig[opos] = next;
            opos++;

            if(remaining > 0)
                buffer[0] = next;
            remaining--;
            length = 1;
        }
        /* 滑动窗口 */
        memmove(&window[0], &window[length], LZ77_WINDOW_SIZE - length);
        /* 将buffer填入窗口,更新窗口 */
        memmove(&window[LZ77_WINDOW_SIZE - length], &buffer[0], length);
    }
    *original = orig;
    return opos;
}
