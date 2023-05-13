#include "myinclude.h"

#define COUNT 20

int main(int argc, char const *argv[]) {

    setbuf(stdout, NULL);

    // szemafor letrehozasa
    int semid = semget(IPC_PRIVATE, 3, IPC_CREAT | 0660);
    if (semid < 0) {
        syserr("semget");
    }

    // kezdeti ertekek megadasa
    short init[3] = {1, 0, 0};
    if (semctl(semid, 0, SETALL, init) < 0) {
        syserr("semctl");
    }

    // szemafor muveletek
    struct sembuf down0 = {0, -1, 0};
    struct sembuf down1 = {1, -1, 0};
    struct sembuf down2 = {2, -1, 0};
    struct sembuf up0 = {0, +1, 0};
    struct sembuf up1 = {1, +1, 0};
    struct sembuf up2 = {2, +1, 0};
    

    pid_t pid1 = fork();
    if (pid1 < 0) {
        syserr("fork");
    }
    if (pid1 == 0) {
        // fiu folyamat (tikk)

        for (int i = 0; i < COUNT; ++i) {
            // szem0 -1 
            if (semop(semid, &down0, 1) < 0) {
                syserr("semop");
            }

            printf("tikk-");

            // szem1 +1
            if (semop(semid, &up1, 1) < 0) {
                syserr("semop");
            }
        }

        exit(EXIT_SUCCESS);
    }

    pid_t pid2 = fork();
    if (pid2 < 0) {
        syserr("fork");
    }
    if (pid2 == 0) {
        // fiu folyamat (takk)

        for (int i = 0; i < COUNT; ++i) {
            // szem1 -1
            if (semop(semid, &down1, 1) < 0) {
                syserr("semop");
            }

            printf("takk-");

            // szem2 +1
            if (semop(semid, &up2, 1) < 0) {
                syserr("semop");
            }
        }

        exit(EXIT_SUCCESS);
    }

    // apa folyamat (tokk)
    for(int i = 0; i < COUNT; ++i) {
        // szem2 -1
        if (semop(semid, &down2, 1) < 0) {
            syserr("semop");
        }

        printf("tokk\n");

        // szem0 +1
        if (semop(semid, &up0, 1) < 0) {
            syserr("semop");
        }
    }

    wait(NULL);
    wait(NULL);

    // szemafor torlese
    if (semctl(semid, 0, IPC_RMID, 0) < 0) {
        syserr("semctl");
    }

    return 0;
}
