#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

/* 信号量头文件. */
#include <semaphore.h>


/* 信号量实现互斥 */
/* 当信号量大于0时，则可以访问，否则将阻塞
   信号量主要用于进程或线程间的同步和互斥这两种典型情况
   信号量的数据类型为：sem_t
   信号量用于互斥：
   开始 -> 初始化信号量为1（都可读） -> P操作（-1 = 0） -> 线程1执行 -> V操作（+1 = 1）
                                -> P操作           -> 线程2执行 -> V操作
   信号量用于同步：
   开始 -> 初始化信号量sem1为1 sem2为0 -> P操作sem1 -> 线程1执行 -> V操作sem2 ->
                                          ^ <----------------------------|
                                    -> P操作sem2 -> 线程2执行 -> V操作sem1 ->
                                          ^ <----------------------------| */



/* 信号量 */
sem_t g_sem;

// 打印机，公共资源
void printer(char *str)
{
    /* P操作 */
    sem_wait(&g_sem);
    while (*str != '\0')
    {
        putchar(*str);
        fflush(stdout);
        str++;

        /* 休眠是为了:让出CPU. */
        usleep(100);
    }
    /* V操作 */
    sem_post(&g_sem);

    printf("\n");
}

// 线程一
void *thread_fun_1(void *arg)
{
    char *str = "hello";
    printer(str); // 打印
}

// 线程二
void *thread_fun_2(void *arg)
{
    // usleep(1);
    char *str = "world";
    printer(str); // 打印
}

int main()
{
    /* 初始化信号量 */
    sem_init(&g_sem, 0, 1);   /* pshared = 0 信号量在线程间共享（常用）；!= 0 信号量在进程间共享 */

    pthread_t tid1, tid2;

    // 创建 2 个线程
    pthread_create(&tid1, NULL, thread_fun_1, NULL);
    pthread_create(&tid2, NULL, thread_fun_2, NULL);

    // 等待线程结束，回收其资源
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    /* 销毁信号量 */
    sem_destroy(&g_sem);

    return 0;
}