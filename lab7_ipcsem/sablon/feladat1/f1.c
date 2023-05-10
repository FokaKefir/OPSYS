/*
  feladat 1 sablon f1.c
*/

#include "myinclude.h"
#include "f12.h"

int main(int argc, char *argv[])
{
    int semid;     //szemafor tömb
    int fid;       //fájl
    char a = 'a';  //ezt írjuk a fájlba
    int count = N; //számláló

    //az f1 hozza létre és inicializálja  a szemaforokat
    //TODO: szemafor tömb lekérés, hozza létre ha nem létezik
    semid = semget(ftok(FTOK_FILE, FTOK_ID), 2, IPC_CREAT | 0660);
    if (semid < 0) {
      syserr("semid");
    }

    //TODO:az init a kezdő értékeket tartalmazza
    short init[2] = {1, 0};

    //TODO: a két művelet f1 számára
    struct sembuf down0 = {0, -1, 0};
    struct sembuf up1 = {1, +1, 0};

    //TODO: szemafor tömb inicializálás
    if (semctl(semid, 0, SETALL, init) < 0) {
      syserr("sem init");
    }

    //TODO: fájl megnyitása írásra, O_WRONLY | O_TRUNC | O_CREAT | O_APPEND
    fid = open(FNAME, O_CREAT | O_WRONLY | O_TRUNC | O_APPEND, 0644);
    if (fid < 0) {
      syserr("file open");
    }

    //íro ciklus
    while (count--)
    {
        //TODO: kritikus szakaszba lép
        if (semop(semid, &down0, 1) < 0) {
          syserr("down");
        }

        //TODO: write
        if (write(fid, &a, 1) != 1) {
          syserr("write");
        }

        //kilép a kritikus szakaszból
        if (semop(semid, &up1, 1) < 0) {
          syserr("up");
        }
    }

    //zárja a fájlt
    close(fid);

    exit(0);
}