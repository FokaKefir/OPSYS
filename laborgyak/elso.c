#include "myinclude.h"


void send_usr1_signal() {
    FILE *fin = fopen("4.pid", "r");
    if (!fin) {
        syserr(
            "fopen"
        );
    }

    int pid4;
    fscanf(fin, "%d", &pid4);

    fclose(fin);

    kill(pid4, SIGUSR1);
}

void fel1(int semid, int *pnum1) {
    // szemafor muveletek
    struct sembuf down1 = {0, -1, SEM_UNDO}; // elso szemafor (0. index) -1-el csokkent
    struct sembuf up2 = {1, +1, 0};

    
    // file megnyitasa
    FILE *fin = fopen("be.txt", "r");
    if (!fin) {
        syserr("fopen");
    }

    int num;
    while (fscanf(fin, "%d", &num) != EOF) {

        // belep a kritikus szekcioba
        if (semop(semid, &down1, 1) < 0) {
            syserr("semop");
        }

        (*pnum1) = num;
        //printf("1: %d\n", num);

        // kilep a kritikus szekciot
        if (semop(semid, &up2, 1) < 0) {
            syserr("semop");
        }
        
    }

    // veget ert a file

    // belep a kritikus szekcioba
    if (semop(semid, &down1, 1) < 0) {
        syserr("semop");
    }

    (*pnum1) = -1;

    // kilep a kritikus szekciot
    if (semop(semid, &up2, 1) < 0) {
        syserr("semop");
    }

    if (fclose(fin) != 0) {
        syserr("fclose");
    }

}

void fel2(int semid, int *pnum1, int *pnum2) {
    // szemafor muveletek
    struct sembuf down2 = {1, -1, SEM_UNDO};
    struct sembuf up3 = {2, +1, 0};

    while (1) {

        // belep a kritikus szekcioba
        if (semop(semid, &down2, 1) < 0) {
            syserr("semop");
        }
        
        (*pnum2) = 2 * (*pnum1);
        //printf("2: %d\n", (*pnum2));


        // kilep a kritikus szekciobol
        if (semop(semid, &up3, 1) < 0) {
            syserr("semop");
        }

        if ((*pnum1) == -1) {
            break;
        }
    }

}

void fel3(int semid, int *pnum2) {
    // szemafor muveletei
    struct sembuf down3 = {2, -1, SEM_UNDO};
    struct sembuf up1 = {0, +1, 0};

    FILE *fout = fopen("ki.txt", "w");
    if (!fout) {
        syserr("fopen");
    }

    int num3;
    while (1) {
        // belep a kritikus szekcioba
        if (semop(semid, &down3, 1) < 0) {
            syserr("semop");
        }

        num3 = (*pnum2) / 2;
        //printf("3: %d\n", num3);

        if (num3 < 0) {
            break;
        }

        fprintf(fout, "%d\n", num3);


        // kilep a kritikus szekciobol
        if (semop(semid, &up1, 1) < 0) {
            syserr("semop");
        }
    }

    if (fclose(fout) != 0) {
        syserr("fclose");
    }

    send_usr1_signal();

}




int main(int argc, char const *argv[]) {
    
    setbuf(stdout,NULL);

    // szemafor letrehozasa
    int semid = semget(IPC_PRIVATE, 3, IPC_CREAT | 0660);
    if (semid < 0) {
        syserr("semget");
    }

    // szemafor inicializalasa
    short init[3] = {1, 0, 0};
    if (semctl(semid, 0, SETALL, init) < 0) {
        syserr("semctl");
    }

    // osztott memoria letrehozasa
    int shmid = shmget(IPC_PRIVATE, 2 * sizeof(int), IPC_CREAT | 0660);
    if (shmid < 0) {
        syserr("shmget");
    }

    // osztott memoria csatolasa
    int *mem = shmat(shmid, NULL, 0);
    if (mem == (void *) -1) {
        syserr("shmat");
    }

    // folyamatok letrehozasa
    pid_t pid2 = fork();
    if (pid2 < 0) {
        syserr("fork");
    }
    if (pid2 == 0) {
        pid_t pid3 = fork();
        if (pid3 < 0) {
            syserr("fork");
        }
        if (pid3 == 0) {
            // folyamat 3
            fel3(semid, mem + 1);
            exit(EXIT_SUCCESS);
        }

        // folyamat 2
        fel2(semid, mem, mem + 1);
        wait(NULL);
        exit(EXIT_SUCCESS);
    }

    // folyamat 1
    fel1(semid, mem);
    wait(NULL);

    // szemafor torlese
    if (semctl(semid, 0, IPC_RMID) < 0) {
        syserr("semctl");
    }


    // osztott memoria lecsatolasa
    if (shmdt((void *) mem) < 0) {
        syserr("shmdt");
    }

    // osztott memoria torlese
    if (shmctl(shmid, IPC_RMID, 0) < 0) {
        syserr("shmctl");
    }
    

    return 0;
}
