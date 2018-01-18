
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

#define FIBER_STACK 8192

int a;
void * stack;

int do_something() {
    printf("This is son, the pid is:%d, the a is: %d\n", getpid(), ++a);
    free(stack);
    exit(1);
}

int main() {
    void * stack;

    a = 1;
    stack = malloc(FIBER_STACK);
    if(!stack) {
        printf("The stack failed\n");
        exit(0);
    }

    printf("creating son thread!!!\n");

    clone(&do_something, (char *)stack + FIBER_STACK, CLONE_VM | CLONE_VFORK, 0);

    printf("This is father, my pid is: %d, the a is: %d\n", getpid(), a);

    return 0;
}
