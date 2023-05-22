/* 
  Sablon a "Tömb feltöltése több folyamat által" feladathoz
  
  A TODO sorok nincsenek beírva!

  Ami meg van oldva:
  1. apa folyamat több fiút indít
  2. apa folyamat leállítja minden fiát TERM jelzéssel
 */

#include "../../myinclude.h"

#define K 5  //fiúk száma
#define N 20 //tömb hossza
#define MS 20 //időzítés kritikus szakasz 

void fiu_feladata(int i, int semid, int *pindex, int *tomb) {
    struct sembuf fiu_down[2] = {{0, -1, 0}, {1, -1, 0}};
    struct sembuf fiu_up[2] = {{0, +1, 0}, {2, +1, 0}};
    
    printf("start: %d fiú pid=%d, csoport=%d\n", i, getpid(), getpgrp());
    while(1){
        // fiu kritikus szekcioba lep
        if (semop(semid, fiu_down, 2) < 0) {
            syserr("semop");
        }

        // fiu a sajat erteket beirja a tombbe
        int index = *pindex;
        tomb[index] = i;
        (*pindex)++;

        // fiu kilep a kritikus szekciobol
        if (semop(semid, fiu_up, 2) < 0) {
            syserr("semop");
        }
        
        // fiu var egy picit
        if (MS != 0) {
            usleep(MS * 1000);
        }
    
    }
    //printf("end: fiú pid=%d\n", getpid());
}


void apa_feladat(int semid, int *pindex, int *tomb) {
    struct sembuf apa_down = {2, -N, 0};
    struct sembuf apa_up = {1, +N, 0};

    // N-szer elvegzi a tomb kiiratasat
    for (int i = 0; i < 10; i++) {
        // apa kritikus szekcioba lep
        if (semop(semid, &apa_down, 1) < 0) {
            syserr("semop");
        }

        // kiirja a tomb tartalmat
        printf("%d. iras: ", i);
        for (int j = 0; j < N; j++)
        {
            printf("%d ", tomb[j]);
        }
        printf("\n");

        // nullazza az indexet
        (*pindex) = 0;

        // apa vegez a kritikus szekcioval
        if (semop(semid, &apa_up, 1) < 0) {
            syserr("semop");
        }
    }
    
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

    // 3 szemafor letrehozasa
    int semid = semget(IPC_PRIVATE, 3, 0660 | IPC_CREAT);
    if (semid < 0) {
        syserr("semget");
    }

    // kezdeti ertek megadasa a szemaforoknak
    short init[3] = {1, N, 0};
    if (semctl(semid, 0, SETALL, init) < 0) {
        syserr("semctl");
    }

    // osztott memoria letrehozasa
    int shmid = shmget(IPC_PRIVATE, (N + 1) * sizeof(int), IPC_CREAT | 0660);
    if (shmid < 0) {
        syserr("shmget");
    } 

    // osztott memoria felcsatolasa
    int *ip = (int *) shmat(shmid, NULL, 0);
    if (ip == (void *) -1) {
        syserr("shmat");
    }


    // fiu folyamatok letrehozasa
    int i;
    for (i = 0; i < K; i++)
    {
        pid = fork();
        if (pid < 0){
            syserr("fork hiba");
        }
        if (pid == 0)
        {
            fiu_feladata(i, semid, ip, ip + 1);
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

    apa_feladat(semid, ip, ip + 1);

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

    // osztott memoria lecsatolasa
    if (shmdt((void *) ip) < 0) {
        syserr("shmdt");
    }

    // szemafor torlese
    if (semctl(semid, 0, IPC_RMID) < 0) {
        syserr("semctl");
    }

    // osztott memoria torlese
    if (shmctl(shmid, IPC_RMID, 0) < 0) {
        syserr("shmctl");
    }

    exit(EXIT_SUCCESS);
}
