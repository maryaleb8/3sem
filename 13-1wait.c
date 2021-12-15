// добавить ифу про то, есть ли core dump при завершении дочерней программы; wait заменить на waitpid
//

#define _GNU_SOURCE //объявили ее и часть кода которая #if def... перерь исполныется
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void proc_into(const char *procname)
{
  printf(
    "%s: PID %d, PPID %d, SID %d\n",
    procname, getpid(), getppid(), getpgid(0), getsid(0)
  );
}

int main(void)
{
  pid_t child_id = fork();
  if (child_id < 0) {
    perror("fork");
    return 1;
  }
  if(child_id == 0){
    proc_info("child try 1");
    sleep(1);
    proc_info("child try 2");
    return 0;
  }
  proc_info("parent");
  int status;
  pid_t res = wait(&status);//статус это куда вернуть значение
  if(WIFEXITED(status)){//сам умер?
    printf("child with PID = %d has existed normally with code %d\n", res, WEXITSTATUS(status));}
  else if (WIFSIGNALED(status)){// прибило сигналом?(нужен иф потому что может быть авершился с core dump)
    
    printf("child with PID = %d has killed by signal %d\n", res, WTERMSID(status));}
  else {
    printf("child with PID = %d has exited with termination status %d\n", res, status);
  }
  return 0;
}
