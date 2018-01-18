
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

#define QLEN 8
#define DEFAULT_SOCKET_PATH "wzy.socket"

int main(int argc, char **argv) {
    int fd, clifd, n;
    const char *socket_path = DEFAULT_SOCKET_PATH;
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
    unlink(socket_path);

    if (bind(fd, (struct sockaddr *) &un, sizeof(un)) < 0) {
        perror("bind error");
        exit(1);
    }

    printf("UNIX Domain Socket bound\n");

    memset(&buf, 0, sizeof(buf));

    if (listen(fd, QLEN) < 0) {
        perror("listen error");
        exit(1);
    }

    while (1) {
        printf("Waiting for client to connect...\n");
        if ((clifd = accept(fd, NULL, NULL)) == -1) {
            perror("accpet error");
            continue;
        }

        while ((n = read(clifd, buf, sizeof(buf))) > 0) {
            printf("read %d bytes: %s\n", n, buf);
        }

        if (n == -1) {
            exit(-1);
        }
        else if (n == 0) {
            printf("END\n");
            close(clifd);
        }
    }

    return 0;
}
