#include<stdio.h>
#include<sys/time.h>

int main()
{
    /* 定义两个结构体 */
    struct timeval start;
    struct timeval end;

    unsigned long timer;
    /* 程序开始之前计时start */
    gettimeofday(&start, NULL);
    printf("hello world!\n");
    /* 程序块结束后计时end */
    gettimeofday(&end, NULL);
    /* 统计程序段运行时间 */
    timer = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("timer = %ld us\n", timer);
    return 0;
}
