// добавить ифу про то, есть ли core dump при завершении дочерней программы; wait заменить на waitpid
//

#define _GNU_SOURCE //объявили ее и часть кода которая #if def... перерь исполныется
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void proc_info(const char *procname)
{
  printf(
    "%s: PID %d, PPID %d, PGID %d, SID %d\n",
    procname, getpid(), getppid(), getpgid(0), getsid(0)
  );
}

int main(void)
{
  pid_t child_id = fork();
  if (child_id < 0) {
    perror("Failure in fork");
    return 1;
  }
  if(child_id == 0){
    proc_info("child try 1");
    sleep(1);
    //proc_info("child try 2");
    return 0;
  }
  proc_info("parent");
  int status;
  pid_t res = waitpid(child_id, &status, WUNTRACED | WCONTINUED);
  //WIFEXITED(status) не равно нулю, если дочерний процесс успешно завершился
  while(WIFEXITED(status) == 0 && WIFSIGNALED(status) == 0) {
    if(res == -1) {
      perror("Failure in waitpid");
      return 2;
    }
    else if (WIFSTOPPED(status))
      printf("child with PID = %d has stopped by signal %d\n", res, WSTOPSIG(status));
    else if (WIFCONTINUED(status))
      printf("child with PID = %d has exited with termination status %d\n", res, status);
  }

  if(WIFEXITED(status)){//сам умер?
    printf("child with PID = %d has exited normally with code %d\n", res, WEXITSTATUS(status));}
  else if (WIFSIGNALED(status)){// прибило сигналом?(нужен иф потому что может быть авершился с core dump)
    if(WCOREDUMP(status))
      printf("child with PID = %d has core dump\n", res);
    else
      printf("child with PID = %d has killed by signal %d\n", res, WTERMSIG(status));
  }
  return 0;
}








//
