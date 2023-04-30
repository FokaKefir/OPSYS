/* 
 * nagyon egyszerű kommunikáció két folyamat között:
 *
 * a message nevű fájl jelenléte jelzi a fiú folyamatnak,
 * hogy az apa befejezett  egy feladatot és ő következik
 * amíg nincs message nevű fájl, a fiú vár
 */

#include "myinclude.h"

int main() {

    pid_t fiu_pid ;
    int fd;

    setbuf(stdout,NULL); //a stdout-ra pufferelés nélkül írunk

    //mi történik ha másodszor futtatjuk a programot
    //és kivesszük az unlink() hívást?
    if ( ! access("message",F_OK) ) { //létezik a fájl?
        printf("torles\n");

        if ( unlink ("message") < 0 ){ /* töröljük  a message  nevű állományt*/
            syserr("nem lehet törölni a message nevű fájlt\n"); 
        }
    }

    fiu_pid = fork();

    if (fiu_pid < 0) {
        syserr("fork");
    } 
    if ( fiu_pid == 0) {
        /* fiu var addig, amig megjelenik a file */
        while ( access("message", F_OK ) == -1){
            usleep(100000); // az usleep-nek mikrószékundumban adjuk meg a várakozási időt
            //100000 * 1 us = 100msec
        }
        printf ("fiú: message megjött\n");
        sleep(1); 
        printf ("fiú kilép\n");
        exit(0);    /* a fiú itt kilép */
    }
    //apa kódja
    printf ("Pid számok, apa=%d fiú=%d\n", getpid(), fiu_pid);

    // apa folyamat kiír kis várakozással
    printf("apa: vár\n");
    sleep(3); //hosszabb időt vár

    printf("apa: message létrejön\n");

    // itt létrehozza a message állományt 
    if ( (fd=open("message", O_CREAT | O_WRONLY | O_TRUNC, 0644))<0){
        syserr("open message");
    }
    close(fd);
    wait(NULL);  //megvárja a fiát
    printf("apa: kilép\n");
    exit (EXIT_SUCCESS);
}

