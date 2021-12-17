/*скопировать содержимое из одного файла в другой через pread pwrite*/
//не же самые ошибки что в предыдущей
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
        fprintf(stderr, "Usage: %s source destination\n", argv[0]);
        return 1;
    }
    struct stat sb;
    if (lstat(argv[1], &sb) < 0){
        perror("lstat");
        return 2;
    }
    if(!S_ISREG(sb.st_mode)) { //слишком страшная строка, макросы if(!S_ISREG(sb.st_mode))
        perror("Not a regular file");
        return 3;
    }
    int fd1 = open(argv[1], O_RDONLY);
    if(fd1 < 0) {
        perror("Failure in opening first file");
        result = 4;
    }
    int fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(fd2 < 0) {
        perror("Failure in opening second file");
        result = 5;
    }

    char text[sb.st_size];
    ssize_t countr, countw;
    off_t offset_r = 0, offset_w = 0;

    while ((countr = pread(fd1, text, sizeof(text), offset_r)) > 0 && result == 0) {
      offset_r += countr;
      while (countr) {
        countw = pwrite(fd2, text, (size_t)countr, offset_w);
        if (countw == -1) {
          perror("Failure in write");
          result = 6;
          break;
        }
        countr -= countw;
        offset_w += countw;
      }
    }
    if (countr == -1) {
      perror("Failure in read");
      result = 7;
    }

    if(close(fd1) < 0) {
        perror("Failure in close first");
        return 8;
    }
    if(close(fd2) < 0) {
        perror("Failure in close second");
        return 9;
    }

    return result;
}
