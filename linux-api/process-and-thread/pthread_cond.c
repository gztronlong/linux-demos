/******************************************************************************
 * 描述：
 *     应用Pthreads条件变量的实例代码，主线程创建三个线程，其中两个为“count”变量做
 * 加法运算，第三个线程监视“count”的值。当“count”达到一个限定值，等待线程准备接收来
 * 自于两个加法线程中一个的信号，等待 线程唤醒后更改“count”的值。程序继续运行直到加法
 * 线程达到TCOUNT的值。最后，主程序打印出count的值。
 ******************************************************************************/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS  3
#define TCOUNT 5       //单线程轮询次数
#define COUNT_LIMIT 7  //发送信号的次数
int count = 0; //全局的累加量

pthread_mutex_t count_mutex;
pthread_cond_t count_threshold_cv;

void *inc_count(void *t) {
    int i;
    long my_id = (long) t;

    for (i = 0; i < TCOUNT; i++) {
        pthread_mutex_lock(&count_mutex);
        count++;
        /*
         * 检查count的值，如果条件满足就发信号给等待线程
         * 注意，此处是用信号量锁定的。
         * */
        if (count < COUNT_LIMIT) {
            printf("inc_count(): thread %ld, count = %d  Threshold reached. ",
                    my_id, count);
            pthread_cond_signal(&count_threshold_cv);
            printf("Just sent signal.\n");
        }
        printf("inc_count(): thread %ld, count = %d, unlocking mutex\n", my_id,
                count);
        pthread_mutex_unlock(&count_mutex);

        /*为线程轮询互斥锁增加延时*/
        sleep(1);
    }
    pthread_exit(NULL);
}

void *watch_count(void *t) {
    long my_id = (long) t;
    printf("Starting watch_count(): thread %ld\n", my_id);

    /*锁定互斥量并等待信号，注意，pthread_cond_wait函数在等待时将自动以自动原子方式
     * 解锁互斥量。还有，请注意，如果等待线程运行到等待函数之前已经满足COUNT_LIMIT的
     * 条件判断，轮询会忽略掉等待函数，
     * */
    while (count < COUNT_LIMIT) {
        pthread_mutex_lock(&count_mutex);
        printf("watch_count(): thread %ld going into wait...\n", my_id);
        pthread_cond_wait(&count_threshold_cv, &count_mutex);
        printf("watch_count(): thread %ld Condition signal received.\n", my_id);

        printf("watch_count(): thread %ld count now = %d.\n", my_id, count);
        pthread_mutex_unlock(&count_mutex);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    int i;
    long t1 = 1, t2 = 2, t3 = 3;
    pthread_t threads[3];
    pthread_attr_t attr;

    /*初始化互斥量和条件变量对象*/
    pthread_mutex_init(&count_mutex, NULL);
    pthread_cond_init(&count_threshold_cv, NULL);

    /*创建线程时设为可连接状态，便于移植*/
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(&threads[0], &attr, watch_count, (void *) t1);
    pthread_create(&threads[1], &attr, inc_count, (void *) t2);
    pthread_create(&threads[2], &attr, inc_count, (void *) t3);

    /* 等待所有线程完成*/
    for (i = 1; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    /*发送信号给监听线程*/
    pthread_cond_signal(&count_threshold_cv); 
    pthread_join(threads[0],NULL);
    printf("Main(): Waited on %d threads. Final value of count = %d. Done.\n",
            NUM_THREADS, count);

    /*清除并退出 */
    pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&count_mutex);
    pthread_cond_destroy(&count_threshold_cv);
    pthread_exit(NULL);
}
