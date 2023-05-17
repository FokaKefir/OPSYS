
/* osztott memoria hasznalata 
 * az apa es fiu folyamatok felvaltva
 * novelik egy valtozo erteket az osztott memoriaban
 *
 * minden ciklusban ellenorzik, hogy a szam paros vagy paratlan
 * az egyik paros a masik mindig paratlan szamokat kell
 * ott talaljon
 *
 * a muveleteket szinkronizalni kell szemaforral
*/

#include "myinclude.h"

#define NUM 4 //ciklusszam

//kiirja a ket szemafor erteket
//segedfuggveny
//holvan: egy sztring, erteke "bent" ha a kritikus szakaszban van a hivo
//semid: szemafor tomb azonosito
void printsem(char *holvan, int semid)
{
    int sem0, sem1;
    //a semctl GETVAL muvelete kiolvassa a szemafor erteket
    //a 0. szemafor erteke
    if ((sem0 = semctl(semid, 0, GETVAL)) < 0)
        syserr("s0");
    if ((sem1 = semctl(semid, 1, GETVAL)) < 0)
        syserr("s1");

    printf("%s : szemafor értékek: %d %d \n", holvan, sem0, sem1);
    return;
}

int main()
{
    int shmid; // osztott mem
    int semid; // szemafor
    int i;
    void *shmem; // erre a mutatora csatoljuk az osztott memoriat
    int *ip;     // ez is az osztott memoriara fog mutatni

    short init[] = {1, 0}; //szemafor kezdo ertekek

    //a negy muvelet alternalo mukodeshez
    struct sembuf up = {0, 1, 0};
    struct sembuf down = {0, -1, 0};
    struct sembuf up1 = {1, 1, 0};
    struct sembuf down1 = {1, -1, 0};

    pid_t pid;

    setbuf(stdout, NULL);

    // osztott memoria 1 int valtozonak
    if ((shmid = shmget(IPC_PRIVATE, sizeof(int), 0660 | IPC_CREAT)) < 0)
        syserr("shmget");

    // 2 szemaforbol allo tomb id-je
    if ((semid = semget(IPC_PRIVATE, 2, 0660)) < 0)
        syserr("semget");

    if (semctl(semid, 0, SETALL, init) < 0) // a ket szemafor beallitasa 0/1
        syserr("semctl");

    printsem("kezdet", semid);
    
    shmem = shmat(shmid, NULL, 0); // attach

    if (shmem == (void *)-1)       
        syserr("shmat");

    // az int mutato hozzarendelese az osztott memoriahoz
    // lehetne ezt egybol hasznalni a shmat() hivasban
    // de jobb ha marad egy void * ami biztosan az osztottra mutat
    ip = (int *) shmem;

    *ip = 0; //kezdoertek, paros szam,eloszor a fiu fogja modositani

    pid = fork();
    if (pid < 0)
        syserr("fork");

    //fiu
    if (pid == 0)
    {
        for (i = 0; i < NUM; i++)
        {
            //kritikus szakaszba lep
            if (semop(semid, &down, 1) < 0)
                syserr("semop 0");

            printsem("fiú bent", semid);
            printf("fiú: a szam:%d\n", *ip);

            //ellenorzi, hogy paros a szam
            if (*ip & 0x0001)
            {
                printf("fiú: hiba, nem páros szám:%d\n", *ip);
                exit(EXIT_FAILURE);
            }
            (*ip)++; // megnoveli
            //kilep a kritikus szakaszbol
            if (semop(semid, &up1, 1) < 0)
                syserr("semop 1");
        }
        //vege: a fiu detach muveletet vegez es kilep
        if (shmdt(shmem) < 0)
            syserr("shmdt"); // detach

        exit(EXIT_SUCCESS);
    }

    // az apa kodja
    for (i = 0; i < NUM; i++)
    {
        if (semop(semid, &down1, 1) < 0)
            syserr("semop 1");

        printsem("apa bent ", semid);
        printf("apa: a szam:%d\n", *ip);

        //? paratlan
        if (!(*ip & 0x0001))
        {
            printf("apa: hiba, nem paratlan szam:%d", *ip);
            exit(1);
        }

        (*ip)++; // megnoveli

        if (semop(semid, &up, 1) < 0)
            syserr("semop 0");
    }

    //levalasszuk a memoriat
    if (shmdt(shmem) < 0)
        syserr("shmdt");

    wait(NULL);

    // vege toroljuk  az IPC objektumokat
    if (shmctl(shmid, IPC_RMID, 0) < 0)
        syserr("shmctl");
    if (semctl(semid, 0, IPC_RMID, 0) < 0)
        syserr("shmctl");

    exit(EXIT_SUCCESS);
}
