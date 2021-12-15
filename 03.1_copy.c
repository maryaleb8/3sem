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
        //close для входлного открывания
        return 3;
    }

    struct stat sb;
    //if (lstat((argv[1], &sb
    lstat(argv[1], &sb);//обработать ошибку, сначала стат, потом открываем, нам нужно только то что открыть нужно
    if ((sb.st_mode & S_IFMT) != S_IFREG) { //слишком страшная строка, макросы if(!S_ISREG(sb.st_mode))
        perror("Not a regular file");
        return 4;
    }

    char text [sb.st_size];//слишком мало памяти, нужно авести буфер размера, нужно сравнить числа которые мы считали и изначальный размер файла
    if (read(fd1, &text, sb.st_size) == -1) {//write all
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
