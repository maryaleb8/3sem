//выводит информацию которая рассказывает о запущенном экземпляре программы credentials

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <grp.h>

int main(void){
    printf(
        "PID %d  PPID %d\nPGID %d SID %d\n",
        getpid(), getppid(), getpgrp(), getsid(0) //про себя знает 0
    );
    int numgroups = getgroups(0, NULL);//If gidsetsize is 0, getgroups() shall return the number
    //of group IDs that it would otherwise return without modifying the array pointed to by grouplist.
    gid_t groups[numgroups];
    if (getgroups(numgroups, groups) == -1) {
        perror("Failure in getgroups");
        return 1;
    }
    printf("Groups: ");
    struct group * gr;
    for(int i = 0; i < numgroups; i++){
        gr = getgrgid(groups[i]);
        const char * answer;
        if(gr == NULL){
          answer = " ";
        }
        else{
          answer = gr->gr_name;
        }
        printf("%d(%s), ", groups[i], answer);
    }
    return 0;

}
