#include "myinclude.h"
#include "randlimit.h"

int N = 6; //ciklusok száma

void fiu1(int semid, int *shm) {
    //első fiú szemafor down művelete
    struct sembuf fiu1_down = {1, -1, 0};
    //mindkét fiú up művelete
    struct sembuf fiuk_up = {0, +1, 0};

    for (int i = 0; i< N; i++) {
        if (semop(semid, &fiu1_down, 1) < 0) {
            syserr("semop");
        }

        (*shm) = i;

        if (semop(semid, &fiuk_up, 1) < 0) {
            syserr("semop");
        }
    }

}

void fiu2(int semid, int *shm) {
    //második fiú szemafor down művelete
    struct sembuf fiu2_down = {2, -1, 0};
    //mindkét fiú up művelete
    struct sembuf fiuk_up = {0, +1, 0};


    for (int i = 0; i< N; i++) {
        if (semop(semid, &fiu2_down, 1) < 0) {
            syserr("semop");
        }

        (*shm) = i;

        if (semop(semid, &fiuk_up, 1) < 0) {
            syserr("semop");
        }
    }
}

void apa(int semid, int *shm) {
    //apa szemafor műveletei
    struct sembuf apa_down = {0, -2, 0};
    struct sembuf apa_up[] = {{1, +1, 0}, {2, +1, 0}};


    for (int i = 0; i< N; i++) {
        if (semop(semid, &apa_down, 1) < 0) {
            syserr("semop");
        }

        int i1 = (*shm);
        int i2 = *(shm + 1);
        printf("1:%d 2:%d\n", i1, i2);

        if (semop(semid, apa_up, 2) < 0) {
            syserr("semop");
        }
    }
}


int main() {
    int semid;
    pid_t pid1, pid2;

    //megoldás 3 szemaforral
    short init[] = {0, 1, 1}; //kezdőértékek, fiúk lépnek először
    
    setbuf(stdout,NULL);

    //TODO: itt megoldani
    semid = semget(IPC_PRIVATE, 3, IPC_CREAT | 0660);
    if (semid < 0) {
        syserr("semget");
    }

    if (semctl(semid, 0, SETALL, init) < 0) {
        syserr("semctl");
    }

    // osztott memoria szegmens
    int shmid = shmget(IPC_PRIVATE, 2 * sizeof(int), IPC_CREAT | 0660);
    if (shmid < 0) {
        syserr("shmget");
    }

    int *ip = (int *) shmat(shmid, NULL, 0);
    if (ip == (int *) -1) {
        syserr("smat");
    }


    pid1 = fork();
    if (pid1 < 0) {
        syserr("fork");
    }
    if (pid1 == 0) {
        // fiu folyamat 1
        fiu1(semid, ip);
        exit(EXIT_SUCCESS);
    }

    pid2 = fork();
    if (pid2 < 0) {
        syserr("fork");
    }
    if (pid2 == 0) {
        // fiu folyamat 2
        fiu2(semid, ip + 1);
        exit(EXIT_SUCCESS);
    }

    // apa folyamat
    apa(semid, ip);
    wait(NULL);
    wait(NULL);

    if (semctl(semid, 0, IPC_RMID) < 0) {
        syserr("semctl");
    }

    if(shmctl(shmid, IPC_RMID, 0) < 0) {
        syserr("shmctl");
    }

    exit(EXIT_SUCCESS);
}