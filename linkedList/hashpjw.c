#include "hashpjw.h"

unsigned int hashpjw(const void *key)
{
    const char *ptr;
    unsigned int val;

    val = 0;
    ptr = key;

    while(*ptr != '\0')
    {
        unsigned int tmp;
        val = (val << 4) + (*ptr);
        tmp = (val & 0xf0000000);
        if(tmp)
        {
            val = val ^ (tmp >> 24);
            val = val ^ tmp;
        }
        ptr++;
    }
    return val % PRIME_TBLSIZ;
}
