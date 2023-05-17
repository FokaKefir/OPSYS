//olvasó folyamat

#include "myinclude.h"
#include "shm.h"

//két szemafort használunk
//kezdőérték s0=1, s1=0
//az olvasó folyamat down: -1->s1
//                   up:   +1->s0

int main( int argc, char * argv[])
{
    int semid, shmid;
    char * mem;    //ide csatoljuk

    //TODO: szemafor műveletek definiálása
    struct sembuf fogyaszto_down = {1, -1, 0};
    struct sembuf termelo_up = {0, +1, 0};
                  
    //TODO: szemafor tömb lekérése
    semid = semget(KEY, 2, 0);
    if (semid < 0) {
        syserr("semget");
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

    while(1) {

        //TODO:
        // 1. down művelet
        if (semop(semid, &fogyaszto_down, 1) < 0) {
            syserr("semop");
        }

        // 2. 1 sor olvasása és kiírása: mem->stdout 
        //ha  a sor "exit" akkor break
        if(strcmp(mem, "exit") == 0 || strcmp(mem, "exit\n") == 0) {
            break;
        }
        printf("%s",mem);

        // 3. up művelet
        
        if (semop(semid, &termelo_up, 1) < 0) {
            syserr("semop");
        }

    }    

    //TODO: osztott memória lecsatolás
    if(shmdt((void *) mem) < 0) {
        syserr("shmctl");
    }


    //TODO: szemafor tömb és osztott memória id törlése
    if(semctl(semid,0,IPC_RMID)<0) syserr("ctl1"); 
    if(shmctl(shmid,IPC_RMID,0)<0) syserr("ctl2"); 



    exit(EXIT_SUCCESS);
}