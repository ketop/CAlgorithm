#include <stdio.h>

#include "encrypt.h"
#include "cbc.h"

int main()
{
    char content[] = "hello, this is mine.";
    long key = 0x12345678901234;
    unsigned char ciphertext[255];
    cbc_encipher((const unsigned char *)content, ciphertext,(const unsigned char *)&key, 20);
    int i;
    for(i = 0; i < 21; i++)
    {
        printf("%c", ciphertext[i]);
    }

    unsigned char plaintext[255];

    printf("\nDecode\n");

    cbc_decipher(ciphertext, plaintext, (const unsigned char *)&key, 20);
    for(i = 0; i < 21; i++)
    {
        printf("%c", *(plaintext + i));
    }

    return 0;
}
