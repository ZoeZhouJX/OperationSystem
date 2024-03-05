#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

/* 线程的 创建 & 退出 & 回收 */

typedef struct StuInfo
{
    int age;
    char sex;
} StuInfo;

int retVal = 100;

void * thread_func(void *arg)
{
    StuInfo * info = (StuInfo *)arg;
    printf("age:%d,\t sex:%c\n", info->age, info->sex);

#if 0
    int val = *(int *)arg;
    printf("val:%d\n", val);
#endif

    printf("i am new thread\n");

    pthread_t tid = pthread_self();
    printf("thread tid:%ld\n", tid);

    pthread_exit((void *)&retVal);

    sleep(3);
}

int main()
{
    pthread_t tid = 0;
    int num = 100;

    StuInfo stu;
    stu.age = 10;
    stu.sex = 'f';

#if 0
    int num = 100;
#endif

    /* 创建线程 */
    int ret = pthread_create(&tid, NULL, thread_func, (void *)&stu);
    if (ret != 0)
    {
        char * err = strerror(ret);
        /* 打印错误码 */
        perror(err);
        _exit(-1);
    }
    usleep(1000);
    printf("main tid:%ld\n", tid);

    /* 阻塞等待回收线程资源 */
    int *retVal = NULL;
    pthread_join(tid, (void *)&retVal);

    printf("hello world\n");

    sleep(3);

    return 0;
}