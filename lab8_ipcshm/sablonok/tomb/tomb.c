/* 
  Sablon a "Tömb feltöltése több folyamat által" feladathoz
  
  A TODO sorok nincsenek beírva!

  Ami meg van oldva:
  1. apa folyamat több fiút indít
  2. apa folyamat leállítja minden fiát TERM jelzéssel
 */

#include "../../myinclude.h"

#define K 3  //fiúk száma
#define N 10 //tömb hossza
#define MS 20 //időzítés kritikus szakasz 

void fiu_feladata(int i)
{
    printf("start: %d fiú pid=%d, csoport=%d\n", i, getpid(), getpgrp());
    while(1){
        usleep(10000);
    }
    printf("end: fiú pid=%d\n", getpid());
}

int main(int argc, char **argv)
{
    int status;
    pid_t pid;
    int children = 0; //elindított fiúk száma
    pid_t * pids = NULL ; //fiúk pid számát tároló tömb mutatója
    int exited=0; //kilépett fiúk száma

    setbuf(stdout,NULL);

    printf("apa pid=%d, csoport=%d\n", getpid(), getpgrp());

    int i;
    for (i = 0; i < K; i++)
    {
        pid = fork();
        if (pid < 0){
            syserr("fork hiba");
        }
        if (pid == 0)
        {
            fiu_feladata(i);
            exit(EXIT_SUCCESS);
        }
        //tömb növelése
        pids = (pid_t*) realloc(pids,(i+1)*sizeof(pid_t));
        if (pids==NULL){
            syserr("pids");
        }
        pids[children++]=pid;
    }

    //apa kód

    usleep(300000); //a fiúk induljanak el, ha nem vár, lehet, hogy még nem futnak 

    //fiúk leállítása
    while(children--){
        if (kill(pids[children], SIGTERM)<0){
            syserr("kill");
        }
    }
    int wpid;
    while (exited != K)
    {
        wpid=wait(&status);
        exited++;
        printf("a pid=%d azonosítójú fiú kilépett\n", wpid);
    }

    free(pids);

    exit(EXIT_SUCCESS);
}
