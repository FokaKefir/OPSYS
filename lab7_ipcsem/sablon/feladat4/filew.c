// az író folyamat

#include "myinclude.h"
#include "file.h"

int main( int argc, char * argv[])
{
    int semid;

    int fid; //fájl azonosító
    int cnt=0; //ciklus számláló
    char fname[] = "a.dat";

    //egy szemafor, kezdőértéke 0
    //minden írás után megemeli a szemafort
    struct sembuf up = { 0, +1 , 0 };  

    //ha meg akarja nézni, hogy a másik kiolvasott mindent
    //ezt arra lehet használni, hogy bevárja a másik folyamatot
    //zéróra vár:
    struct sembuf wzero = { 0, 0, 0 }; 
                  
    //TODO: szemafor tömb létrehozás

    //TODO: szemafor inicializálás 0-ra

    //TODO: fájl nyitása írásra, csonkolás

    int k; 
    for (k=0; k<N ; k++){

        //TODO: minden ciklusban kiírja a cnt-t a fájlba
        //és megemeli a szemafort

        //TODO: ha kiírt annyit, amekkora a szemafor maximális értéke, 
        //bevárja az olvasó folyamatot
        //semaphore max value = 32767

        //usleep(500000);//késletetés teszteléshez, így lassabban fog futni mint az olvasó
        cnt++;
    }    

    if (close(fid)<0) syserr("close");

    exit(0);
}