/*без цикла то же самое*/
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[]) {
    int result = 0;

    if(argc != 3) {
        fprintf(stderr, "Usage: %s filename string\n", argv[0]);
        return 1;
    }

    int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(fd < 0) {
        perror("Failure in opening");
        return 2;
    }
    if(dprintf(fd, "%s", argv[2]) < 0) {
        perror("Failure in writing");
        result = 3;
    }
    if(close(fd) < 0) {
        perror("Failure in close");
        result = 4;
    }

    return result;
}
