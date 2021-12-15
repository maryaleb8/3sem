// использовать sigaction
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

volatile int g_last_signal;//означает что может меняться так как в цикле

void sig_handler(int signum)
{
  g_last_signal = signum;
  //exit(1);
}

int main(void)
{
  signal(SIGINT, sig_handler);
  signal(SIGQUIT, sig_handler);// есть еще убивающие ctrl+z, kill, sigterm, sighub, sigtstp
  while(1) {
    pause();
    printf("last_signal = %d (%s)\n", g_last_signal, strsignal(g_last_signal));
  }
  return 0;
}
