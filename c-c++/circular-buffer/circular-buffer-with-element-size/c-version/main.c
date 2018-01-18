
#include "circular_buffer.h"
#include <stdio.h>

int main() {
    char tmpBuf[1024];
    char helloBuf[] = "hello world"; // 11
    size_t nread;
    size_t i;

    struct circular_buffer *buf = circular_buffer_init(1024);

    printf("capacity: %d\n", circular_buffer_capacity(buf));
    printf("size: %d\n", circular_buffer_size(buf));

    printf("\n");
    circular_buffer_push(buf, helloBuf, 11);
    printf("size: %d\n", circular_buffer_size(buf));

    circular_buffer_push(buf, tmpBuf, 1024);
    printf("size: %d\n", circular_buffer_size(buf));

    printf("\n");
    circular_buffer_clear(buf);
    printf("size: %d\n", circular_buffer_size(buf));

    printf("\n");
    for (i = 0; i < 10; i++) {
        circular_buffer_push(buf, helloBuf, 11);
        circular_buffer_push(buf, helloBuf, 5);
    }

    for (i = 0; i < 20; i++) {
        circular_buffer_pop(buf, tmpBuf, &nread);
        tmpBuf[nread] = '\0';
        printf("nread: %d string: %s\n", nread, tmpBuf);
    }
    printf("\n");
    circular_buffer_pop(buf, tmpBuf, &nread);
    printf("nread: %d string: %s\n", nread, tmpBuf);

    return 0;
}
