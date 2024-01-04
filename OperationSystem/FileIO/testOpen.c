#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int main()
{
    int fd = open("./hello.txt", O_RDWR, 0644);
    if (fd == -1)
    {
        perror("open error");
    }
    return 0;
}