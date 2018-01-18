#include <glib.h>

GMainLoop *mloop;
static GMutex *mutex = NULL;
static gboolean t1_end = FALSE;
static gboolean t2_end = FALSE;

void *run_1(void *tmp) {
    int i ;

    //for(i=0; i<arg->max; i++)
    for(i=0; i<11; i++) {
        if(g_mutex_trylock(mutex) == FALSE) {
            g_print("%d : thread 2 locked the mutex \n", i);
            g_mutex_unlock(mutex);
        }
        else {
            g_usleep(10);
        }
    }
    t1_end = TRUE;
    return NULL;
}

void *run_2(void *tmp) {
    int i;
    //for(i=0; i<arg->max; i++)
    for(i=0; i<11; i++) {
        if(g_mutex_trylock(mutex) == FALSE) {
            g_print("%d : thread 1 locked mutex \n", i);
            g_mutex_unlock(mutex);
        }
        else
            g_usleep(10);
    }
    t2_end = TRUE;
    return NULL;
}

void *run_3(void *arg) {
    for(;;) {
        if(t1_end && t2_end) {
            g_main_loop_quit(mloop);
            break;
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if(!g_thread_supported())
        g_thread_init(NULL);
    mloop = g_main_loop_new(NULL, FALSE);

    mutex = g_mutex_new();
    g_thread_create(run_1, NULL, TRUE, NULL);
    g_thread_create(run_2, NULL, TRUE, NULL);
    g_thread_create(run_3, NULL, TRUE, NULL);

    g_main_loop_run(mloop);
    g_print("Exit main loop\n");
    g_mutex_free(mutex);
    g_print("Free mutex\n");

    return 0;
}
