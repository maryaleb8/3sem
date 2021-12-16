/*записать в указанный файл указанное содержимое open, write, close*/
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

ssize_t write_all(int fd, const void *buf, size_t count)
{
    size_t bytes_written = 0;
    while(bytes_written < count) {
        ssize_t res = write(fd, buf + bytes_written, count - bytes_written);
        if(res < 0) {
            return res;
        }
        bytes_written += (size_t)res;
    }
    return (ssize_t)bytes_written;
}


int main(int argc, char* argv[]) {
    int result = 0;

    if(argc != 3) {
        fprintf(stderr, "Usage: %s filename string\n", argv[0]);
        result = 1;
    }
    int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(fd < 0) {
        perror("Failure in opening");
        result = 2;
    }

    if(write_all(fd, argv[2], strlen(argv[2])) < 0) {
        perror("Failure in writing");
        result = 3;
    }

    if(close(fd) < 0) {
        perror("Failure in close");
        result = 4;
    }

    return result;
}
