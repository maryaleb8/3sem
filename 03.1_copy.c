/*скопировать содержимое из одного файла в другой*/
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if(argc != 3) {
        fprintf(stderr, "Usage: %s filename string\n", argv[0]);
        return 1;
    }

    int fd1 = open(argv[1], O_RDONLY);
    if(fd1 < 0) {
        perror("Failure in opening first file");
        return 2;
    }
    int fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(fd2 < 0) {
        perror("Failure in opening second file");
        return 3;
    }

    struct stat sb;
    lstat(argv[1], &sb);
    if (((sb.st_mode) & (S_IFMT)) != S_IFREG) {
        perror("Not a regular file");
        return 4;
    }

    char text [sb.st_size];
    if (read(fd1, &text, sb.st_size) == -1) {
        perror("Failed to read first file");
        return 5;
    }

    if(dprintf(fd2, "%s", text) < 0) {
        perror("Failure in writing");
        close(fd1);
        close(fd2);
        return 6;
    }
    if(close(fd1) < 0) {
        perror("Failure in close first");
        return 7;
    }
    if(close(fd2) < 0) {
        perror("Failure in close second");
        return 8;
    }

    return 0;
}
