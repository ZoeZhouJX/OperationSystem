#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

/* 生产者/消费者模型 */

/* 条件变量的优缺点 
   相较于mutex而言，条件变量可以减少竞争。
   如直接使用mutex，除了生产者、消费者之间要竞争互斥量以外，
   消费者之间也需要竞争互斥量。但如果汇聚（链表）中没有数据，
   消费者之间竞争互斥锁是无意义的。
   有了条件变量机制以后，只有生产者完成生产，才会引起消费者
   之间的竞争。提高了程序效率。
*/



/* 锁资源 */
pthread_mutex_t g_mutex;

/* 条件变量 */
pthread_cond_t g_notEmpty;  /* 生产者给消费者发 */
pthread_cond_t g_notFull;   /* 消费者给生产者发 */

/* 链表结点 */
typedef struct Node
{
    int val;
    struct Node * next;
} Node;

/* 链表的头指针 */
Node * head = NULL;

/* 生产者 */
void * produceFunc(void *arg)
{
    int count = 1;
    while (1)
    {
        Node *newNode = (Node *)malloc(sizeof(Node) * 1);
        if (newNode == NULL)
        {
            perror("malloc error");
            pthread_exit(NULL);
        }
        memset(newNode, 0, sizeof(Node) * 1);
        newNode->val = count++;

        /* 加锁 */
        pthread_mutex_lock(&g_mutex);

        /* 头插 */
        newNode->next = head;
        head = newNode;

        printf("====produce:%ld\t%d\n", pthread_self(),newNode->val);
        pthread_mutex_unlock(&g_mutex);

        /* 信号通知 */
        pthread_cond_signal(&g_notEmpty);

        /* 休眠 */
        usleep(3000);
    }

    /* 线程退出 */
    pthread_exit(NULL);
}

/* 消费者 */
void * consumeFunc(void *arg)
{
    while (1)
    {
        /* 加锁 */
        pthread_mutex_lock(&g_mutex);

        /* 当没有数据的时候，就条件等待 */
        while (head == NULL)
        {
            /* 等待条件变量 */
            pthread_cond_wait(&g_notEmpty, &g_mutex);
        }

        /* 消费数据 */
        Node * delNode = head;
        head = head->next;

        printf("consume:%ld,\t%d\n", pthread_self(), delNode->val);

        /* 释放内存 */
        if (delNode)
        {
            free(delNode);
            delNode = NULL;
        }
        pthread_mutex_unlock(&g_mutex);

        usleep(6000);
    }
}

int main()
{
    /* 初始化锁和条件变量 */
    pthread_mutex_init(&g_mutex, NULL);
    pthread_mutex_init(&g_notEmpty, NULL);
    pthread_mutex_init(&g_notFull, NULL);

    /* 生产者线程 */
    pthread_t produce_tid;
    /* 消费者线程 */
    pthread_t consume_tid;

    /* 生产者 */
    int ret = pthread_create(&produce_tid, NULL, produceFunc, NULL);
    if (ret != 0)
    {
        printf("thread create error");
        exit(-1);
    }

    /* 消费者 */
    int ret = pthread_create(&consume_tid, NULL, consumeFunc, NULL);
    if (ret != 0)
    {
        printf("thread create error");
        exit(-1);
    }

    /* 回收线程资源 */
    pthread_join(produce_tid, NULL);
    pthread_join(produce_tid, NULL);

    /* 释放锁资源和条件变量资源 */
    pthread_mutex_destroy(&g_mutex);
    pthread_cond_destroy(&g_notEmpty);
    pthread_cond_destroy(&g_notFull);

    return 0;
}