#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <linux/unistd.h>

#define N 5

pid_t gettid() {
    // Get the process ID, need a system call. Kernel use this ID to schedule.
    return syscall(__NR_gettid);
}

void *thread_func(void *arg) {
    printf("thread started, pid = %d\n", gettid());
    while (1) {
        sleep(1);
    }
}

int main(void) {
    int i;
    pthread_t tid[N];

    for (i = 0; i < N; i++) {
        pthread_create(&tid[i], NULL, thread_func, NULL);
    }

    sleep(1);
    for (i = 0; i < N; i++) {
        printf("tid = %lu\n", tid[i]);
    }
    sleep(10);

    return 0;
}
