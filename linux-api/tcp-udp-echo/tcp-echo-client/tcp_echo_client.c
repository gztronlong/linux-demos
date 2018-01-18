#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>

#define BUFFER_SIZE 1024 // buf size

int main(int argc, char *argv[]) {
    int conn_fd, port, ret = 0;
    const char *server_ip;
    char buf[BUFFER_SIZE];
    struct sockaddr_in serv_addr;

    // Check the arguments.
    if (argc != 3) {
        printf("Usage: %s server_ip port_number\n", argv[0]);
        return -1;
    }

    // Initialize address.
    server_ip = argv[1];
    port = atoi(argv[2]);
    memset(&serv_addr, 0, sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = inet_addr(server_ip);

    // Create socket.
    if ((conn_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return -1;
    }

    // Connect server.
    if ((connect(conn_fd, (struct sockaddr *)&serv_addr,
                sizeof(struct sockaddr))) < 0) {
        perror("connect");
        goto err;
    }

    bzero(buf, sizeof(buf));

    // Get input from keyboard.
    if ((read(STDIN_FILENO, buf, sizeof(buf))) < 0) {
        perror("read");
        goto err;
    }

    // Send data to server.
    if ((send(conn_fd, buf, sizeof(buf), 0)) < 0) {
        perror("send");
        goto err;
    }

    // Receive data from server.
    memset(buf, 0, sizeof(buf));
    if ((ret = recv(conn_fd, buf, BUFFER_SIZE, 0)) < 0) {
        perror("recv");
        goto err;
    }

    printf("Receive data from server: %s\n", buf);

    close(conn_fd);
    return 0;

err:
    close(conn_fd);
    return -1;
}
