#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    clock_t begin, end;
    double cost;
    //开始记录
    begin = clock();
    /*待测试程序段*/
    printf("hello world!\n");
    //结束记录
    end = clock();
    cost = (double)(end - begin);
    printf("constant CLOCKS_PER_SEC is: %ld, time cost is: %lf secs", CLOCKS_PER_SEC, cost);
}
