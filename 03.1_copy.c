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

    char text [sb.st_size];//слишком мало памяти, нужно авести буфер размера, нужно сравнить числа которые мы считали и изначальный размер файла
    int count = 0;
    int a;
    while((a = (int)read(fd1, &text, (size_t)sb.st_size)) != -1 && a != 0){
        count += a;
        if(dprintf(fd2, "%s", text) < 0) {
            perror("Failure in writing");
            result = 7;
            break;
        }
    }
    
    int sizef = (int)sb.st_size;
    if(count != sizef)
    {
        perror("Failure in file size");
        result = 8;
    }

    /*if (read(fd1, &text, (size_t)sb.st_size) == -1) {
        perror("Failed to read first file");
        result = 6;
    }

    if(dprintf(fd2, "%s", text) < 0) {
        perror("Failure in writing");
        result = 7;
    }*/
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
