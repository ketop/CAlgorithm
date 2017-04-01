#include <stdlib.h>
#include <stdio.h>

#include "bits.h"
#include "cbc.h"
#include "encrypt.h"

/*
 *
 *  Usually we add one random block of data to the beginning of the plaintext. This is
 *  so that even when an adversary has some idea what the first block of plaintext
 *  contains, it cannot be used to start replicating the chaining sequence. This block is
 *  called the initialization vector. We encipher it normally, without any feedback,
 *  then use it as the feedback when enciphering and deciphering the first real block
 *  of plaintext.
 *
 * . It assumes that the first block of plaintext is actually
 *   the 64-bit initialization vector.
 *
 * */
void cbc_encipher(const unsigned char *plaintext, unsigned char *ciphertext, const unsigned char *key, int size)
{
    unsigned char temp[8];
    int i;
    des_encipher(&plaintext[0] , &ciphertext[0], key);
    i = 8;
    while( i < size )
    {
        bit_xor(&plaintext[i], &ciphertext[i - 8], temp, 64);
        des_encipher(temp, &ciphertext[i], NULL);
        i = i + 8;
    }
    return;
}

void cbc_decipher(const unsigned char *ciphertext, unsigned char *plaintext, const unsigned char *key, int size)
{
    unsigned char temp[8];
    int i;
    des_decipher(&ciphertext[0], &plaintext[0], key);
    i= 8;
    while(i < size)
    {
        des_decipher(&ciphertext[i], temp, NULL);
        bit_xor(&ciphertext[i - 8], temp, &plaintext[i], 64);
        i = i + 8;
    }
    return;
}
