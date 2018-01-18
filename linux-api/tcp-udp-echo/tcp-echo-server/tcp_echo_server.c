#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 1024 // buf size
#define LISTENQ 1 // the max quote

int main(int argc, char *argv[]) {
    struct sockaddr_in serv_addr;
    char recv_buf[BUFFER_SIZE];
    int sock_fd, conn_fd, ret, i, port, optval = 1;

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
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return -1;
    }

    // Set bind port reuse.
    if ((setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&optval,
                sizeof(int))) < 0) {
        perror("setsockopt");
        goto err;
    }

    // Bind the port.
    if ((bind(sock_fd, (struct sockaddr *)&serv_addr,
        sizeof(struct sockaddr_in))) < 0) {
        perror("bind");
        goto err;
    }

    // Listen.
    if ((listen(sock_fd, LISTENQ)) < 0) {
        perror("listen");
        goto err;
    }

    // Wait and accept client to connect.
    socklen_t cli_len = sizeof(struct sockaddr_in);
    while (1) {
        // Accept client's connect request.
        if ((conn_fd = accept(sock_fd, (struct sockaddr *)&serv_addr,
                        &cli_len))< 0) {
            perror("accept");
            return -1;
        }

        // Receive data from client.
        if ((ret = recv(conn_fd, recv_buf, BUFFER_SIZE, 0)) < 0) {
            perror("recv");
            return -1;
        }

        printf("Receive data from client: %s\n", recv_buf);

        // Transform the data.
        for (i = 0; i < ret; i++) {
            recv_buf[i] = toupper(recv_buf[i]);
        }

        // Send data back to client.
        if ((send(conn_fd, recv_buf, sizeof(recv_buf), 0)) < 0) {
            perror("send");
            return -1;
        }

        close(conn_fd);
    }

    close(conn_fd);
    close(sock_fd);
    return 0;

err:
    close(sock_fd);
    return -1;
}
