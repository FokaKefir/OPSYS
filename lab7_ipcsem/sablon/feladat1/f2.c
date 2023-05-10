/*
  feladat 1 sablon f2.c
*/
#include "myinclude.h"
#include "f12.h"

int main( int argc, char * argv[])
{
    int semid; //szemafor tömb 
    int fid;  //fájl
    char b  = 'b'; //ezt írjuk a fájlba
    int count = N; //számláló

    //TODO:a két szemafor művelet f2 számára
    struct sembuf down1 = {1, -1, 0};
    struct sembuf up0 = {0, +1, 0};

    //TODO: a szemafor tömb lekérése, adjon hibát ha nem létezik
    //az f1 által létrehozva
    semid = semget(ftok(FTOK_FILE, FTOK_ID), 2, 0);
    if (semid < 0) {
      syserr("semid");
    }

    //TODO: fájl nyitása írásra, adjon hibát ha nem létezik
    //ne csonkolja
    fid = open(FNAME, O_WRONLY | O_APPEND);
    if (fid < 0) {
      syserr("file open");
    }

    //íro ciklus
    while (count--)
    {
        //TODO: kritikus szakaszba lép
        if (semop(semid, &down1, 1) < 0) {
          syserr("down");
        }

        //TODO: write
        if (write(fid, &b, 1) != 1) {
          syserr("write");
        }

        //TODO:kilép a kritikus szakaszból
        if (semop(semid, &up0, 1) < 0) {
          syserr("up");
        }

    }

    //TODO: törli a szemafor tömböt
    //ez a folyamat használja utoljára
    if (semctl(semid, 0, IPC_RMID, 0) < 0) {
      syserr("sem rm");
    }

    //zárja a fájlt
    close(fid);
    exit(EXIT_SUCCESS);

} 