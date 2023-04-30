/* 
 *  a fork függvény kezelése - apa és fiú folyamat
 *  külön kód részekén futnak
 */

#include "myinclude.h"

int main() {
    printf ("\n");
    pid_t fiu_pid ; //az apa és fiú pid számát itt tároljuk
    pid_t apa_pid ;

    /* a kimenet pufferelését megszüntetjük, így a printf
       függvény azonnal kiír miután meghívódik
       */
    setbuf (stdout,NULL);

    apa_pid = getpid(); //apa pid száma      

    fiu_pid = fork();           //folyamat indítás

    if ( fiu_pid < 0 ) {                  //hibakezelés
        syserr("fork");
    } 

    if ( fiu_pid == 0) {
        // fiu kod resze 
        printf ("Fiú: apa pid %d, fiú pid %d\n", (int) apa_pid, (int) getpid());

        printf("fiú vár\n"); 
        sleep(4);                          // fiú vár 4 másodpercet

        printf("Fiú: kilép\n");
        exit(0);
    } else {    
        /* apa kód része */     
        //     sleep(2);
        printf("Apa: apa pid %d, fiú pid %d\n", (int) apa_pid, (int) fiu_pid);

        wait(NULL);      //megvárja a fiút, NULL a paraméter ha nem kell a kilépési érték

        printf("Apa: kilép\n");
        exit(0);
    }    

    printf("ide nem érnek el!\n");
    return 0;
}

