#include <sys/resource.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main()
{
    struct rusage ru;
    memset(&ru, 0 , sizeof(struct rusage));
    getrusage(RUSAGE_SELF, &ru);
    long  secs = ru.ru_utime.tv_sec + ru.ru_stime.tv_sec;
    long usec = ru.ru_utime.tv_usec + ru.ru_stime.tv_usec;

    printf("%ld, %ld", secs, usec);
    return 0;
}
