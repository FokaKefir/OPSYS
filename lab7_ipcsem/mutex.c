/* 
 * peldaprogram a P es V szemafor muveletek implementalasara
 * IPC szemaforokkal
 * 
 * P - megprobalja lehuzni a szemafort
 * V - elengedi a lehuzott szemafort
 *
 * a programban egy apa 3 fiu folyamatot indit, mindegyik ugyanazt a
 * feladatot kell elvegezze
 *
 * egyszerre csak egy fiu vegezheti a feladatot
 *
 * futtassuk le a peldat, utana vegyuk ki a 
 *     sleep(t);
 * sort a fiuk feladatabol
  hogyan valtozik meg a kritikus szakaszba valo lepes es miert igy?
 */

#include "myinclude.h"

//P muvelet a semid azonositoju szemaforral
void P(int semid)
{
    //muvelet:  -1 a 0 indexu szemaforba
    struct sembuf down = {0, -1, 0};

    if (semop(semid, &down, 1) < 0)
        syserr("semop");
    return;
}

//V muvelet a semid azonositoju szemaforral
void V(int semid)
{
    //muvelet:  +1 a 0 indexu szemaforba
    struct sembuf up = {0, 1, 0};

    if (semop(semid, &up, 1) < 0)
        syserr("semop");
    return;
}

//a feladat amit a fiuk kell vegezzenek felvaltva
//egy karaktert irnak ki (str), mindegyik mas karaktert
//varnak t masodpercig a kritikus szakaszban
//a feladatot k-szor vegzik el
void fiu_feladata(int semid, char *str, int t, int k)
{
    int i;
    int pid1;
    pid1 = getpid();
    for (i = 0; i < k; i++)
    {
        P(semid); //probalja lehuzni a szemafort
        printf("%d: a %s-t kiíró fiú bejutott a kritikus részbe és vár: %d szekundumot\n", pid1, str, t);
        // sleep(t);
        printf("kiir %s\n", str);
        printf("%d: a %s-t kiíró fiú kilép a kritikus részből\n", pid1, str);
        V(semid); //visszaallitja a szemafort
    }
    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv)
{
    int semid;
    pid_t p;
    setbuf(stdout, NULL);

    //egy szemafortomb lekerese amelyben 1 szemafor lesz
    if ((semid = semget(IPC_PRIVATE, 1, 0660 | IPC_CREAT)) < 0)
        syserr("semget");

    printf("semid=%d\n", semid);

    //a szemafor kezdeti erteke 0
    if (semctl(semid, 0, SETVAL, 1) < 0)
        syserr("semctl1");

    if ((p = fork()) < 0)
        syserr("fork");

    if (p == 0)
    {                                   //elso fiu
        fiu_feladata(semid, "a", 1, 3); //feladat utan a fiu a fuggvenyben kilep
    }

    if ((p = fork()) < 0)
        syserr("fork");

    if (p == 0)
    { //masodik fiu
        fiu_feladata(semid, "b", 1, 2);
    }

    if ((p = fork()) < 0)
        syserr("fork");

    if (p == 0)
    { //harmadik fiu
        fiu_feladata(semid, "c", 1, 4);
    }

    wait(NULL);
    wait(NULL);
    wait(NULL); //apa mindharmat megvarja

    if (semctl(semid, 0, IPC_RMID, 0) < 0) //szemafor tomb torlese
        syserr("semctl2");

    printf("apa kilep\n");

    exit(EXIT_SUCCESS);
}
