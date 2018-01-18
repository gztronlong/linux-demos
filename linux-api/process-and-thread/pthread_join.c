#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

struct char_print_parms {
    char character;
    int count;
};

void *char_print (void* parameters) {
    struct char_print_parms*  p = (struct char_print_parms*)parameters;
    int i;

    for (i = 0; i < p->count; ++i)
        printf("%c\n", p->character);

    return NULL;
}

int main () {
    pthread_t thread1_id;
    pthread_t thread2_id;
    struct char_print_parms thread1_args;
    struct char_print_parms thread2_args;

    thread1_args.character = 'x';
    thread1_args.count = 6;
    pthread_create(&thread1_id, NULL, &char_print, &thread1_args);

    thread2_args.character = 'o';
    thread2_args.count = 4;
    pthread_create(&thread2_id, NULL, &char_print, &thread2_args);

    //sleep(1);
    pthread_join(thread1_id, NULL);
    pthread_join(thread2_id, NULL);

    return  0;
}