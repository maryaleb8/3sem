/*скопировать содержимое из одного файла в другой через pread pwrite*/
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
        fprintf(stderr, "Usage: %s source destination\n", argv[0]);
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

    char text[sb.st_size];
    ssize_t countr, countw;
    off_t offset_r = 0, offset_w = 0;

    while ((countr = pread(fd1, text, sb.st_size, offset_r)) > 0) {
      offset_r += countr;
      while (countr) {
        countw = pwrite(fd2, text, countr, offset_w);
        if (countw == -1) {
          perror("Failure in write");
          return 5;
        }
        countr -= countw;
        offset_w += countw;
      }
    }
    if (countr == -1) {
      perror("Failure in read");
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
