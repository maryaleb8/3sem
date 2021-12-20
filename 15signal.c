// использовать sigaction
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sched.h>

volatile int g_last_signal;//означает что может меняться так как в цикле

void sig_handler(int signum, siginfo_t * siginfo, void * x) {//ругается, что неиспользованы 2 и 3
    g_last_signal = signum;
}

int main(void)
{
  struct sigaction sig = {0};
  sig.sa_sigaction = sig_handler;

  if (sigaction(SIGTSTP, &sig, NULL) == -1) {
    perror("sigaction(SIGTSTP)");
    return 1;
  }
  if (sigaction(SIGINT, &sig, NULL) == -1) {
    perror("sigaction(SIGINT)");
    return 2;
  }
  if (sigaction(SIGQUIT, &sig, NULL) == -1) {
    perror("sigaction(SIGQUIT)");
    return 3;
  }
  if (sigaction(SIGHUP, &sig, NULL) == -1) {
    perror("sigaction(SIGHUP)");
    return 4;
  }
  if (sigaction(SIGTERM, &sig, NULL) == -1) {
    perror("sigaction(SIGTERM)");
    return 5;
  }

  while(1) {
    pause();
    printf("last_signal = %d (%s)\n", g_last_signal, strsignal(g_last_signal));
  }
  return 0;
}
