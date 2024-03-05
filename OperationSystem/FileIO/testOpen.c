#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

/* 文件描述符0（stdin）表示标准输入（Standard Input）
   文件描述符1（stdout）表示标准输出（Standard Output）
   文件描述符2（stderr）表示标准错误输出（Standard Error）*/

int main()
{
    /* 打开文件 */
    int fd1 = open("./hello.txt", O_RDWR | O_CREAT, 0644);
    if (fd1 == -1)
    {
        perror("open error");
    }
    printf("fd1 = %d\n", fd1);   // fd1 = 3

    int fd2 = open("./hello.txt", O_RDWR | O_CREAT, 0644);
    if (fd2 == -1)
    {
        perror("open error");
    }
    printf("fd2 = %d\n", fd2);  // fd2 = 4

    int fd3 = open("./hello.txt", O_RDWR | O_CREAT, 0644);
    if (fd3 == -1)
    {
        perror("open error");
    }
    printf("fd3 = %d\n", fd3);  // fd3 = 5

    int fd4 = open("./hello.txt", O_RDWR | O_CREAT, 0644);
    if (fd4 == -1)
    {
        perror("open error");
    }
    printf("fd4 = %d\n", fd4);  //fd4 = 6

    /* 关闭文件fd1. */
    close(fd1);

    int fd5 = open("./hello.txt", O_RDWR | O_CREAT, 0644);
    if (fd5 == -1)
    {
        perror("open error");
    }
    printf("fd5 = %d\n", fd5);  // fd5 = 3

    return 0;
}