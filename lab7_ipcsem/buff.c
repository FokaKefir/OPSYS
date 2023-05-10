/* 
 * a klasszikus fogyaszto / temelo feladat 
 *
 * 3 pufferbe valo iras szimulalasat vegezzuk
 */

#include "myinclude.h"

#define N 3       // a buffer hossza
#define COUNT 10  //hany ciklust vegeznek a termelo es fogyaszto folyamatok

// segedfuggveny
// kiirja a semafor tomb allapotat
void status(int id);

int main() {
    int sem_id;  // a szemafor tomb
    pid_t pid;
    int i;

    // 3 szemafor van a tombben
    // 1: a mutex szemafor, ez biztositja,
    //    hogy egyszerre csak 1 folyamat vegezzen puffer muveletet
    // 2: empty: hany szabad puffer van
    // 3: full:  hany foglalt puffer van

    // kezdoallapot:                               mutex  empty  full
    //                                               1     N      0
    //
    // egy puffer foglalasara "termel":              0     N-1    1
    //
    // jelzes barmely muvelet utan:                  1     N-1    1
    //
    // minden puffer foglalt:                        1     0      N

    //harom szemafor muveletunk lesz:
    //
    // 1 termel
    struct sembuf termel[3] = {
        {0, -1, SEM_UNDO},  // mutex: down
        {1, -1, SEM_UNDO},  // empty: -1
        {2, 1, SEM_UNDO}    // full: +1
    };
    // 2 fogyaszt
    struct sembuf fogyaszt[3] = {
        {0, -1, SEM_UNDO},  // mutex: down
        {1, 1, SEM_UNDO},   // empty: +1
        {2, -1, SEM_UNDO}   // full: -1
    };

    // 3 termeles vagy fogyasztas utan jelezni kell
    struct sembuf jelez = {0, 1, SEM_UNDO};  // mutex: up

    // a szemaforok kezdo ertkei: mutex=1, empty=N, full=0

    short start[] = {1, N, 0};

    setbuf(stdout, NULL);  // stdout puaffereles nelkul

    // kerek egy 3 elembol allo szemafor tombot
    // 0: mutex , 1: empty, 2: full

    if ((sem_id = semget(IPC_PRIVATE, 3, IPC_CREAT | 0660)) == -1) {
        syserr("semget");
    }

    // beallitom a szemafor tomb kezdoertekeit
    if (semctl(sem_id, 0, SETALL, start) == -1) {
        syserr("semctl");
    }

    pid = fork();

    if (pid < 0) {
        printf("\nfork hiba");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {  // a fiu processz lesz a fogyaszto

        for (i = 0; i < COUNT; i++) {
            // usleep(10000); //10 ms  var
            if (semop(sem_id, fogyaszt, 3) < 0) {  // elfogyaszt egy puffert
                syserr("fogyaszt");
            }

            printf("fiú fogyaszt: -1 -> buffer ");
            // belepett  a kritikus szakaszba
            status(sem_id);                      // kiirja a szemafort
            if (semop(sem_id, &jelez, 1) < 0) {  // befejezi, kilep
                syserr("jelez1");
            }
            usleep(100000);  //lassitas
        }
        exit(EXIT_SUCCESS);  // fiu kilep
    } else {                 // az apa processz a termelo

        for (i = 0; i < COUNT; i++) {
            //harom szemaforon vegzett muvelet
            if (semop(sem_id, termel, 3) < 0) {
                syserr("termel");
            }
            // belep a kritikus szakaszba
            printf("apa termel: +1 -> buffer ");
            status(sem_id);
            if (semop(sem_id, &jelez, 1) < 0) {
                syserr("jelez2");
            }
        }
    }

    wait(NULL);  //megvarja, hogy a fia kilepjen

    //az apa torli a szemafor tombot
    if (semctl(sem_id, 0, IPC_RMID, NULL) < 0) {
        syserr("semctl");
    }
    exit(EXIT_SUCCESS);
}

//allapotkiiro rutin
// !!! csak a kritikus szakaszban van ertelme meghivni
void status(int id) {
    unsigned short sem[3];
    // kiolvasom minden szemafor allapotat
    if (semctl(id, 0, GETALL, sem) == -1) {
        syserr("semctl");
    }
    //kiirom hany puffer van tele es hany uress
    printf(" helyzet: full=%d empty=%d\n", sem[2], sem[1]);
    printf("------------------------\n");
    return;
}
