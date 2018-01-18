#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>

#define BUFFER_SIZE 1024 // buf size

int main(int argc, char *argv[]) {
    int conn_fd, port;
    char buf[BUFFER_SIZE];
    socklen_t addr_len;
    struct sockaddr_in serv_addr;

    // Check the arguments.
    if (argc != 2) {
        printf("Usage: %s portnumber\n", argv[0]);
        return -1;
    }

    // Initialize address.
    port = atoi(argv[1]);
    memset(&serv_addr, 0, sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Create udp socket.
    if ((conn_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        goto err;
    }

    bzero(buf, sizeof(buf));

    // Get input from keyboard.
    if ((read(STDIN_FILENO, buf, sizeof(buf))) < 0) {
        perror("read");
        goto err;
    }

    // Send data to server.
    if ((sendto(conn_fd, buf, sizeof(buf), 0, (struct sockaddr *)&serv_addr,
                sizeof(struct sockaddr_in))) < 0) {
        perror("sendto");
        goto err;
    }

    // Receive data from server.
    bzero(buf, sizeof(buf));
    addr_len = sizeof(struct sockaddr_in);
    if ((recvfrom(conn_fd, buf, sizeof(buf), 0, (struct sockaddr *)&serv_addr,
                &addr_len)) < 0) {
        perror("recvfrom");
        goto err;
    }

    printf("Receive data from server: %s\n", buf);

    close(conn_fd);
    return 0;

err:
    close(conn_fd);
    return -1;
}
