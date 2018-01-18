#include <stdio.h>
#include <pthread.h>

void *compute_factorial(void *arg) {
    int n = *((int*)arg);
    int i = 1;
    int factorial = 1;

    for (i = 1; i <= n; i++) {
        factorial *= i;
    }

    return (void*)factorial;
}

int main () {
    pthread_t thread;
    int number = 10;
    int factorial = 0;

    pthread_create(&thread, NULL, &compute_factorial, &number);
    printf("Computing factorial of %d.\n", number);

    pthread_join(thread, (void*)&factorial);
    printf("The factorial of %d is %d.\n", number, factorial);

    return  0;
}
