#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char* argv[])
{
    int fd = open("/dev/meme", O_RDWR, 0600);
    if (fd < 0) {
        perror("open");
        return (-1);
    }

    if (write(fd, "helloworld", 10) < 0) {
        perror(0);
        return (-1);
    }

    return 0;
}