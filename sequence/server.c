#include "myinclude.h"
#include "sequence.h"

//a sor azonosítója külső statikus változó,
//azért hogy a fájlban levő függvényből is elérjük
static int qid;

//ez a függvény törli a sort, minden exit() hívás után
//lefut
void remove_queue()
{
    printf("svr: exit, sor törölve\n");
    if (msgctl(qid,IPC_RMID,NULL)<0){
        syserr("svr: RMID");
    }
}

//szerver program
int main (int argc, char * argv[])
{
    setbuf(stdout,NULL);

    //az atexit() függvénnyel be lehet állítani egy void 
    //paraméterrel rendelkező függvényt, ami lefut minden exit()
    //hívás után, kilépés előtt
    //oldjuk meg, hogy INT vagy TERM jelzésre is hívódjon meg a függvény
    if (atexit(remove_queue)!=0){
        syserr("svr: atexit");
    }

    //itt megoldani a szerver feladatot
}
