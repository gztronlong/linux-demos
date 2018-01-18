#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *print_xs(void *unuse) {
    while (1) {
        printf("x\n");
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t thread_id;
    pthread_create(&thread_id,  NULL, &print_xs, NULL);

    while (1) {
        printf("o\n");
        sleep(1);
    }
    return 0;
}
