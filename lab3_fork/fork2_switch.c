/*
 *  a fork függvény kezelése switch szerkezettel
    a köd ugyanaz mint a fork2.c
 */

#include "myinclude.h"

int main() {
    printf("\n");
    pid_t fiu_pid;  // az apa és fiú pid számát itt tároljuk
    pid_t apa_pid;

    /* a kimenet pufferelését megszüntetjük, így a printf
       függvény azonnal kiír miután meghívódik
       */
    setbuf(stdout, NULL);

    apa_pid = getpid();  // apa pid száma

    fiu_pid = fork();  // folyamat indítás

    switch (fiu_pid)
    {
            case -1:
                syserr("fork");  // fork nem sikerült
                break; //a break pontok feleslegesek, mert minden ág végére exit() vagy _exit() jön 
            case 0:  // fiú kódja
                printf("Fiú: apa pid %d, fiú pid %d\n", (int) apa_pid, (int)getpid());

                printf("Fiú vár\n");
                sleep(4);  // fiú vár 4 másodpercet

                printf("Fiu: kilép\n");
                exit(0);
                break;

            default:  // apa kódja
                printf("Apa: apa pid %d fiú pid %d\n", (int) apa_pid, (int) fiu_pid);

                wait(NULL);  // megvárja a fiút, ha nem kell a kilépési érték
                            // akkor NULL a paraméter

                printf("Apa: kilép\n");
                exit(0);
                break;
        }

    printf("ide nem érnek el!\n");
    return 0;
}
