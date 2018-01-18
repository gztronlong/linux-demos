#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t mutexsum;
long long sum = 0;

void *compute_fact(void *arg) {
    int n = (int)arg;
    int i = 1;
    long long fact = 1;

    for (i = 1; i <= n; i++) {
        fact *= i;
    }

    pthread_mutex_lock (&mutexsum);
    sum += fact;
    printf("Add %lld to sum\n", fact);
    pthread_mutex_unlock (&mutexsum);

    return NULL;
}

int main(int argc, char *argv[]) {
    int i = 0;
    int n = 14;
    pthread_t callThd[n];

    pthread_mutex_init(&mutexsum, NULL);

    for (i = 1; i <= n; i++) {
        pthread_create(&callThd[i], NULL, compute_fact, (void *)i);
    }

    for(i = 1; i <= n; i++) {
        pthread_join(callThd[i], NULL);
    }

    printf("Sum = %lld\n", sum);
    pthread_mutex_destroy(&mutexsum);
    return 0;
}
