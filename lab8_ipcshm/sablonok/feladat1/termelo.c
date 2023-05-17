// az író folyamat

#include "myinclude.h"
#include "shm.h"

//két szemafort használunk
//kezdőérték s0=1, s1=0
//az író folyamat down: -1->s0
//                up:   +1->s1

int main( int argc, char * argv[])
{
    int semid, shmid;
    char * mem;    //ide csatoljuk

    //TODO: szemafor műveletek definiálása
    struct sembuf termelo_down = {0, -1, 0};
    struct sembuf fogyaszto_up = {1, +1, 0};
                  
    //TODO: szemafor tömb létrehozás
    semid = semget(KEY, 2, IPC_CREAT | 0660);
    if (semid < 0) {
        syserr("semget");
    }

    //TODO: szemafor inicializálás
    short init[2] = {1, 0};
    if (semctl(semid, 0, SETALL, init) < 0) {
        syserr("semctl");
    }

    //TODO: osztott memória id lekérése
    shmid = shmget(KEY, SIZE * sizeof(char), IPC_CREAT | 0660);
    if (shmid < 0) {
        syserr("shmget");
    }

    //TODO: osztott memória felcsatolása
    mem = (char *) shmat(shmid, NULL, 0);
    if (mem == (char *) -1) {
        syserr("shmat");
    }

    while(1){
        //TODO:
        // 1. down művelet
        if (semop(semid, &termelo_down, 1) < 0) {
            syserr("semop");
        }
        // 2. 1 sor olvasása stdin->mem, max 1024 byte!
        if (fgets(mem, SIZE, stdin) == NULL) {
            strcpy(mem, "exit");
            if (semop(semid, &fogyaszto_up, 1) < 0) {
                syserr("semop");
            }
            break;
        }

        // 3. up művelet
        //ha  a sor "exit" akkor break
        if(strcmp(mem, "exit") == 0 || strcmp(mem, "exit\n") == 0) {
            if (semop(semid, &fogyaszto_up, 1) < 0) {
                syserr("semop");
            }
            break;
        }
        if (semop(semid, &fogyaszto_up, 1) < 0) {
            syserr("semop");
        }
        
    }

    //osztott memória lecsatolás
    if(shmdt((void *) mem) < 0) {
        syserr("shmctl");
    }


    exit(EXIT_SUCCESS);
}