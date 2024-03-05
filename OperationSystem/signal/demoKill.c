#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>

#define SIZE 5
int main()
{
    /* 创建进程 */
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork error");
        _exit(-1);
    }
    else if (pid == 0)
    {
        printf("child id : %d\n", getpid());
        /* 子进程 */
        int idx = 0;
        for (idx = 0; idx < SIZE; idx++)
        {
            printf("i am son process\n");
            sleep(1);
        }
    }
    else
    {
        /* 打印pid */
        printf("pid:%d\n", pid);
        /* 父进程 */
        printf("i am parent process parent id : %d\n", getpid());
        sleep(2);

        printf("kill sub process now\n");
        kill(pid, SIGINT);
    }

    while (1)
    {
        sleep(3);
    }

    return 0;
}