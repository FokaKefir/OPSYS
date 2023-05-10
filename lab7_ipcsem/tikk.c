/* szinkronizalas szemaforokkal
 * az apa es fiu folyamatoknak felvaltva kell a tikk es takk
 * szavakat kiirniuk 
 * */

#include "myinclude.h"

#define COUNT 4  // COUNT x fognak tikk-takk-olni

/*
strace -f -e trace="semget,semtimedop,semctl" ./tikk >/dev/null 
*/

int main() {
    int sem_id;  // a szemafor tomb id-je
    pid_t pid;
    int i;

    // az up, down az apat vezerlo, az up1 down1 a fiut vezerlo szemaforok
    // definiciojuk:
    // struktura: hanyadik szemafor, muvelet, flag=0 vagy IPC_NOWAIT vagy SEM_UNDO
    struct sembuf down = {0, -1, 0};
    struct sembuf up = {0, 1, 0};
    struct sembuf down1 = {1, -1, 0};
    struct sembuf up1 = {1, 1, 0};

    fflush(stdout);  // kiuritem az stdout bufferet, hogy helyes kimenetet lassak
    setbuf(stdout, NULL);

    //kerek egy 2 elembol allo szemafor tombot, privat mert csak apa fiu hasznalja
    if ((sem_id = semget(IPC_PRIVATE, 2, IPC_CREAT | 0660)) == -1) {
        syserr("semget");
    }
    printf("semget: semget sikerult: id = %d\n", sem_id);

    // beallitom a 0-as szemafor kezdoerteket 0 ra az 1-est 1-re
    // az 1-es szemafornak van 1 erteke, tehat eloszor a down1 hajthato vegre
    short kezdo[] = {0, 1};
    if (semctl(sem_id, 0, SETALL, kezdo) == -1) {
        syserr("semctl");
    }

    //igy tudom kiirni a szemafor pillanatnyi erteket:
    printf("a szemafor erteke=%d\n", semctl(sem_id, 1, GETVAL));

    pid = fork();
    if (pid < 0) {
        printf("\nfork hiba");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {  //a fiu folyamat ciklusa
        pid_t pid1;
        pid1 = getpid();
        for (i = 0; i < COUNT; i++) {
            if (semop(sem_id, &down1, 1) < 0)  // probalkozas
                syserr("down1");
            fprintf(stdout, "%d tikk\n", pid1);  // kritikus resz
            if (semop(sem_id, &up, 1) < 0)       // probalkozas
                syserr("up");
        }
        exit(0);
    } else {  //apa
        pid_t pid2;
        pid2 = getpid();
        for (i = 0; i < COUNT; i++) {
            if (semop(sem_id, &down, 1) < 0)  // probalkozas
                syserr("down");
            //sleep(1);
            fprintf(stdout, "%d takk\n", pid2);
            if (semop(sem_id, &up1, 1) < 0)  // probalkozas
                syserr("up1");
        }

        wait(NULL);

        // az apaban torlom a szemafort: ha eloszor a fiu tikk-el, akkor az apa utoljara

        if (semctl(sem_id, 0, IPC_RMID, 0) < 0)  // torlom a szemafort
            syserr("semctl");
    }

    exit(EXIT_SUCCESS);
}
