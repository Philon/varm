#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char* argv[])
{
    int fd = open("/dev/meme", O_RDWR, 0777);
    if (fd < 0) {
        perror("open");
        return (-1);
    }

    if (write(fd, "helloworld", 11) < 0) {
        perror("write");
        return (-1);
    }

    char buffer[128] = {0};
    if (read(fd, buffer, 20) < 0) {
        perror("read");
        return (-1);
    }
    printf("read from meme: '%s'\n", buffer);

    close(fd);

    return 0;
}