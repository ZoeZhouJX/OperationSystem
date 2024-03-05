#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define BUFFER_SIZE 1024

int main(int argc, char const *argv[]) // 通过映射实现父子进程间通信
{
    int fd = open("./mmap.txt", O_RDWR | O_CREAT, 0644); // 创建|打开映射文件
    if (fd == -1)
    {
        perror("open error");
        _exit(-1);
    }

    int pidfd = fork(); // 创建进程

    if (pidfd == -1) // 创建进程失败
    {
        perror("fork error");
        _exit(-1);
    }

    if (pidfd == 0) // 子进程的操作
    {
        /* 子进程创建映射 */
        void *ptr = mmap(NULL, BUFFER_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
        if (ptr == MAP_FAILED)
        {
            perror("mmap error");
            _exit(-1);
        }

        strcpy((char *)ptr, "666");
        close(fd);
    }

    if (pidfd > 0) // 父进程的操作
    {
        /* 父进程使用映射 */
        void *ptr = mmap(NULL, BUFFER_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
        if (ptr == MAP_FAILED)
        {
            perror("mmap error");
            _exit(-1);
        }

        printf("父进程接受到子进程创建的映射文件:%s\n", (char *)ptr);

        /* 解除映射 */
        int ret = munmap(ptr, BUFFER_SIZE);
        if (ret == -1)
        {
            perror("munmap error");
            _exit(-1);
        }
    }

    return 0;
}