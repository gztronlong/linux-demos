#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    struct sockaddr_in serv_addr;
    char recv_buf[BUFFER_SIZE];
    int port, i, ret, sock_fd;
    socklen_t addr_len;

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

    // Create socket.
    if ((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        goto err;
    }

    // Bind the local port.
    if ((bind(sock_fd, (struct sockaddr *)&serv_addr,
                sizeof(struct sockaddr_in))) < 0) {
        perror("bind");
        goto err;
    }

    // Receive client data and echo.
    while (1) {
        // Receive data.
        addr_len = sizeof(struct sockaddr_in);
        if ((ret = recvfrom(sock_fd, recv_buf, sizeof(recv_buf), 0,
                        (struct sockaddr *)&serv_addr, &addr_len)) < 0) {
            perror("recvfrom");
            goto err;
        }

        printf("Receive message from client: %s\n", recv_buf);

        // Transform data.
        for (i = 0; i < ret; i++) {
            recv_buf[i] = toupper(recv_buf[i]);
        }

        // Send back data.
        if ((sendto(sock_fd, recv_buf, ret, 0, (struct sockaddr *)
                        &serv_addr, sizeof(struct sockaddr_in))) < 0) {
            perror("sendto");
            goto err;
        }
    }

    close(sock_fd);
    return 0;

err:
    close(sock_fd);
    return -1;
}
