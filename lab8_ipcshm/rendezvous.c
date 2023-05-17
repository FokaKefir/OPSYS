/* 
 * A 3.c Rendezvous feladat megoldasa (olvassuk el a teljes feladatot!)
 *
 * forditas:
 *  gcc -Wall rendezvous.c randlimit.c -o rendezvous
 *
 * egy apa es ket fiu
 * a 2 fiu folyamat elvegez egy-egy muveletet, es mindketto ott hagyja
 * az eredmenyt az osztott memoriaban 
 *
 * mivel random ideig alszanak, nem lehet tudni melyik er hamarabb a 
 * talalkozasi ponthoz
 * 
 * szamaforok: 2 szemafor s0, s1
 *             kezdoertekek  {0, 0}
 *             1. fiut blokkolja: s0 
 *             2. fiut blokkolja: s1 
 *
 * használat:
 *
 *      gcc -Wall rendezvous.c -o rendezvous
 *      ./rendezvous
 *
 * */

#include "myinclude.h"

//a szemafor UP muveletet megvalosito
//fuggveny
//parameterek:  1. szemafor objektum id, 2. szemafor index
void up(int semid, int semno)
{
    struct sembuf up = {0, +1, 0};
    up.sem_num = semno;

    if (semop(semid, &up, 1) < 0)
        syserr("up");
}

//a szemafor DOWN muveletet megvalosito
//fuggveny
//parameterek:  1. szemafor objektum id, 2. szemafor index
void down(int semid, int semno)
{
    struct sembuf down = {0, -1, 0};
    down.sem_num = semno;

    if (semop(semid, &down, 1) < 0)
        syserr("down");
}

int main()
{
    int semid, shmid;
    pid_t p1, p2;
    int szam1, szam2; //ezeket a szamokat kell osszeadni
    int no;           //index az osztott memoriaban

    int *ip; //itt lesznek a kozos valtozok
    int sec; //mennyit alszik

    short init[] = {0, 0}; //kezdő érték a szemaforoknak

    setbuf(stdout, NULL); //nincs stdout puffereles

    //szemafor tomb  objektum, 2 szemafor
    if ((semid = semget(IPC_PRIVATE, 2, 0660 | IPC_CREAT)) < 0)
        syserr("semget");
    if (semctl(semid, 0, SETALL, init) < 0)
        syserr("init");

    //osztott memoria objektum, 2 int
    if ((shmid = shmget(IPC_PRIVATE, 2 * sizeof(int), 0660 | IPC_CREAT)) < 0)
        syserr("shmget");

    //memoria csatolas, a mutatot oroklik a fiai is
    if ((ip = (int *)shmat(shmid, NULL, 0)) == (void *)-1)
        syserr("shmat");

    //az elso fiu bemeneti adatai
    no = 0;    //rekesz indexe = 0
    szam1 = 1; //+ 2 szam
    szam2 = 2;

    if ((p1 = fork()) < 0)
        syserr("fork1");

    if (p1 == 0) //elso fiu
    {
        *(ip + no) = szam1 + szam2; //elvegzi a szamitast

        printf("az első 1 szekundumot alszik\n");
        sleep(1); //alszik

        up(semid, 1);   //a 1-s szemafor a 2-es fiue, most mar futhat
        down(semid, 0); //megvarja a jelzest a masiktol

        //atveszi a masik szamitasat
        printf("fiu1: bemenet: %d %d, a másik folyamat összege: %d\n", szam1, szam2, *(ip + !no));

        if (shmdt((void*)ip) < 0)
            syserr("dt");
        exit(EXIT_SUCCESS);
    }

    no = 1;
    szam1 = 3;
    szam2 = 4; //a masodik fiu bemeneti adatai
               //rekesz index 1 + 2 szám
    if ((p2 = fork()) < 0)
        syserr("fork1");

    //ugyanazokat a muveleteket vegzi mint az elso, csak a
    //szemafor kezeles szimmetrikus
    if (p2 == 0) //masodik fiu
    {
        *(ip + no) = szam1 + szam2;

        printf("a második 2 szekundumot alszik\n");
        sleep(2);

        up(semid, 0); // a 0-s szemafor az 1-es fiue
        down(semid, 1);

        printf("fiú 2: bemenet: %d %d, a másik folyamat összege: %d\n", szam1, szam2, *(ip + !no));

        if (shmdt((void*)ip) < 0)
            syserr("dt");
        exit(EXIT_SUCCESS);
    }

    wait(NULL);
    wait(NULL); //apa megvarja fiait

    if (shmdt((void*)ip) < 0)
        syserr("dt");

    //torli az IPC objektumokat
    if (semctl(semid, 0, IPC_RMID, NULL) < 0)
        syserr("semctl");
    if (shmctl(shmid, IPC_RMID, NULL) < 0)
        syserr("shmctl");

    exit(EXIT_SUCCESS);
}
