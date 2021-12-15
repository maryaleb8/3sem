//id функция

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(void){
  printf(
      "PID %d, PPID %d, PGID %d, SID %d\n",
      getpid(), getppid(), getuid(0), getgid(0) //про себя ознаает 0
  );
  printf("UID %d, GID %D\n", getuid(), getgid());
}
