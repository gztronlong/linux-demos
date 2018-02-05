
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

int main()
{
    struct timeval now;

    gettimeofday(&now, NULL );
    struct tm *ts = localtime(&now.tv_sec);

    char time_str[256];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", ts);

    printf("%s\n", time_str);

    return 0;
}
