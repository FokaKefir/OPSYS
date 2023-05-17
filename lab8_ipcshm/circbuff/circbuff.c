/* a klasszikus fogyaszto / termelo feladat 
 * a termelo folyamat egy cirkularis pufferbe ir
 * a fogyaszto onnan olvas
 *
 * a termelo egy szoveget ir at karakterenkent
 *
 * a pufferben 3 hely van - ez allithato az N -el
 *
 * */

#include "myinclude.h"
#include "colors.h"

#define N 3 // a buffer hossza

#include "pufferfun.h"


void status(int id); // allapot kiiro fuggveny, def. a file vegen

int main()
{
    int sem_id, shm_id;         // a szemafor es memoria azonosito
    pid_t pid;
    char ch;

    struct puffer *puff; //a puffer cime

    char text[] = "Ezzel a karaktersorral dolgozunk."; //ASCII szoveg, nem UTF-8 

    int i;

    /* 
       szemafor hasznalat: harom szemafor van
       mutex: 1/0 ertekkel, a puffereket egyszerre egy folyamat erheti el 
       empty: N/N-1,..0 ertekkel: hany szabad hely van
       full:  0/1,2,..N ertekkel: hany foglalt

       kezdoertek: 
            mutex=1
            empty=N
            full=0 
    */

    //termelo muvelet, a termelo folyamat ezzel lep be
    // egyszerre a mutex és empty csokkentese kell sikeruljon, ha sikerul a full +1-et kap
    struct sembuf termel[3] = {
        {0, -1, SEM_UNDO}, // mutex: down
        {1, -1, 0}, // empty: -1
        {2, +1, 0}   // full: +1
    };
    //fogyaszto muvelet
    //egyszerre a mutex es full csokkentese kell sikeruljon, ha sikerul empty +1-et kap
    struct sembuf fogyaszt[3] = {
        {0, -1, SEM_UNDO}, // mutex: down
        {1, +1, 0},  // empty: +1
        {2, -1, 0}  // full: -1
    };

    //a fogyasztas vagy termeles utani kilepes +1 a mutexbe
    struct sembuf jelez[1] = {{0, 1, 0}}; // mutex: up

    setbuf(stdout, NULL);

    // kerek egy 3 elembol allo szemafor tombot
    // szem. indexek: 0: mutex , 1: empty, 2: full

    if ((sem_id = semget(IPC_PRIVATE, 3, IPC_CREAT | 0660)) == -1)
        syserr("semget");

    // beallitom a szemafor tomb kezdoertekeit
    short init[] = {1, N, 0};
    if (semctl(sem_id, 0, SETALL, init) == -1)
        syserr("init");

    // a puffer elokeszitese
    if ((shm_id = shmget(IPC_PRIVATE, sizeof(struct puffer), IPC_CREAT | 0660)) == -1)
        syserr("semget");

    if ((puff = (struct puffer *) shmat(shm_id, NULL, 0)) == (void *)-1)
        syserr("shmat");

    //mundket mutatot 0-ra allitom

    puff->input = 0;
    puff->output = 0;

    pid = fork();
    if (pid < 0)
    {
        syserr("fork");
    }

    if (pid == 0)
    {
        // a fiu processz lesz a fogyaszto
        while (1)
        {
            if (semop(sem_id, fogyaszt, 3) < 0) // elfogyaszt egy puffert
                syserr("semop1");
            ch = get_puff(puff);
            printf(ANSI_COLOR_GREEN "fiu: -1 -> fogyaszt <- %c\n" ANSI_COLOR_RESET, ch);
            // belepett  a kritikus szakaszba
            status(sem_id);                  // kiirja a szemafort
            if (semop(sem_id, jelez, 1) < 0) // befejezi, kilep
                syserr("semop2");

            if (ch == '.'){
                break; // vege ha . jott
            }

            usleep(100000);
            // ezzel kesleltetni lehet a fiut 100ms-al => a pufferek megtelnek
            // kilep
        }

        if(shmdt((void *)puff)<0) syserr("dt");
        exit(EXIT_SUCCESS);
    }
    else
    {
        // az apa processz a termelo
        for (i = 0; i < strlen(text); i++)
        {
            if (semop(sem_id, termel, 3) < 0)
                syserr("semop3");
            // belep a kritikus szakaszba
            char c = text[i]; // szoveg karakterei egyenkent
            printf(ANSI_COLOR_RED "apa: +1 -> termel -> %c\n" ANSI_COLOR_RESET, c);
            put_puff(puff, c);
            status(sem_id);
            if (semop(sem_id, jelez, 1) < 0)
                syserr("semop4");
        }
    }
    wait(NULL); //megvarja, hogy a fia fejeze be
    if(shmdt((void *)puff)<0) syserr("dt");
    //az apa torli a szemafor tombot
    if (semctl(sem_id, IPC_RMID, 0) < 0)
        syserr("semctl");
    if (shmctl(shm_id, IPC_RMID, 0) < 0)
        syserr("shmctl");
    exit(0);
}

//allapotkiiro rutin
void status(int id)
{
    unsigned short sem[3];
    // kiolvasom minden szemafor allapotat
    if (semctl(id, 0, GETALL, sem) == -1)
    {
        perror("semctl");
        exit(1);
    }
    //kiirom hany puffer van tele es hany uress
    printf("------------------------\n");
    printf("full=%d empty=%d\n", sem[2], sem[1]);
    printf("------------------------\n");
    return;
}
