#include <stdlib.h>
#include <stdio.h>

int count()
{
    printf("this is a count\n");
    return 5;
}

void prina(int *p ,int i)
{
    for(; i >= 0; --i)
    {
        printf("%d\n", *(p+i));
    }

}

int main()
{
    int *p;
    int unique[4] = {1, 2, 3, 6};
    p = unique;

    prina(p, count());
    return 0;
}
