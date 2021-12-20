//выводит количество запусков (считая текущий), отслеживая их количество
//нуждо устанавливать блокировку на файл (не flock, a lockf)
#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <unistd.h>


int main(void) {
    int result = 0;

    int fd1 = open("counter.txt", O_RDWR | O_CREAT, S_IRWXU | S_IRGRP | S_IROTH);//user can everything, other only read
    if(fd1 == -1) {
        perror("Failure in opening counter file");
        return 1;
    }
    if(flock(fd1, LOCK_EX) == -1) {
        perror("Failure in flock");
        close(fd1);
        return 2;
    }
    FILE* counter = fdopen(fd1, "r");
    int number = 0;
    if(!fscanf(counter, "%d", &number)) {
        perror("Failure in fscanf");
        result = 3;
    }
    if(fclose(counter) < 0) {
        perror("Failure in close by filename");
    }
    counter = fdopen(fd1, "w+");

    number++;

    if(fprintf(counter, "%d", number) < 0){
        perror("Failure in fprintf");
        result = 4;
    }
    if(flock(fd1, LOCK_UN) == -1) {
        perror("Failure in unlocking");
        result = 5;
    }
    if(close(fd1) < 0) {
        perror("Failure in close by fd");
        return 6;
    }
    if(fclose(counter) < 0) {
        perror("Failure in close by filename");
        return 7;
    }
    return result;
}
