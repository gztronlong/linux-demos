
/*
 * Thanks to http://blog.csdn.net/wzy_1988/article/details/44928691
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

char *socket_path="wzy.socket";

int main(int argc, char **argv) {
    int fd;
    struct sockaddr_un un;
    char buf[100];

    if (argc > 1)
        socket_path = argv[1];

    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        perror("socket error");
        exit(1);
    }

    memset(&un, 0, sizeof(un));
    un.sun_family = AF_UNIX;
    strncpy(un.sun_path, socket_path, sizeof(un.sun_path) - 1);

    if (connect(fd, (struct sockaddr *) &un, sizeof(un)) < 0) {
        perror("connect error");
        exit(-1);
    }

    while (scanf("%s", buf) != EOF) {
        if (write(fd, buf, sizeof(buf)) < 0) {
            perror("write error");
            exit(-1);
        }
    }

    return 0;
}
