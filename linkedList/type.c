#include <stdio.h>



int main()
{
    char *s = "hello world!\n";
    printf("%s\n",s);
    printf("int type: %ld bytes\n", sizeof(int));
    printf("void * pointer type: %ld bytes\n", sizeof(void *));
    printf("int * pointer type: %ld bytes\n", sizeof(int *));

    return 0;
}
