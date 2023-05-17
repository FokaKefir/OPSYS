/* 
  apa folyamat leállítja minden fiát TERM jelzéssel
 */

#include "myinclude.h"

#define N 3

void fiu_feladata()
{
    printf("start: fiú pid=%d, csoport=%d\n", getpid(), getpgrp());
    while(1){
        sleep(1);
    }
    printf("stop: fiú pid=%d\n", getpid());
}

int main(int argc, char **argv)
{
    int status;
    pid_t pid;
    int children = 0; //elindított fiúk száma
    pid_t * pids ; //fiúk pid száma
    int exited=0; //kilépett fiúk száma

    setbuf(stdout,NULL);

    pids = (pid_t*) malloc(N*sizeof(pid_t));
    if (pids==NULL){
        syserr("pids");
    }

    printf("apa pid=%d, csoport=%d\n", getpid(), getpgrp());

    setbuf(stdout, NULL);
    int i;
    for (i = 0; i < N; i++)
    {
        pid = fork();
        if (pid < 0){
            syserr("fork hiba");
        }
         
        pids[children++]=pid;

        if (pid == 0)
        {
            fiu_feladata();
            exit(EXIT_SUCCESS);
        }
    }

    sleep(1); //a fiúk induljanak el, ha nem vár, nincs akinek jelzést küldeni 

    //fiúk leállítása
    while(children--){
        if (kill(pids[children], SIGTERM)<0){
            syserr("kill");
        }
    }

    int wpid;
    while (exited != N)
    {
        wpid=wait(&status);
        exited++;
        printf("    a pid=%d azonosítójú fiú véget ért\n", wpid);
    }

    exit(EXIT_SUCCESS);
}
