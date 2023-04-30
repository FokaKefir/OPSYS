#include "myinclude.h"
/*
 * egyszerű csővezeték apa és fiú folyamat között
 * a fiú folyamat átír egy kis sztringet az apának
 * csővezetéken keresztül
 * fordítás:
 *    gcc -Wall pipe.c -o pipe
 *   
 * 
 */

#define MAX 255 /* puffer hossza az olvasashoz */

int main() {
    pid_t pid;
    int pfd[2]; /* a csovezetek azonositoi */

    char a[] = "Helló!";  //ezt a kis sztringet irjuk at a csovon
    char b[MAX];          //puffer
    /* a pipe fuggveny hozza letre a csovezetek azonositoit 
     * ezt a fork() elott le kell futtatni 
     */
    if (pipe(pfd) < 0) {
        syserr("pipe");
    }

    if ((pid = fork()) < 0) {
        syserr("fork");
    }
    //fiu folyamat:
    if (pid == 0) {
        /*szabaly: a folyamat lezarja a csovezetek azon felet amelyet nem hasznal*/
        if (close(pfd[0]) < 0) { /* irni fog, az olvaso felet lezarja */
            syserr("close");
        }
        /* atirja a sztringet, megy a \0 is */
        if (write(pfd[1], a, strlen(a) + 1) < 0) {
            syserr("write");
        }

        /* a csovet hasznalat utan le kell zarni, ekkor tudja meg az olvaso
           folyamat, hogy vége az adat atvitelnek */
        if (close(pfd[1]) < 0) {
            syserr("close1");
        }
        exit(0);
    } else {                      //apa folyamat:
        if (close(pfd[1]) < 0) {  //olvas, tehat bezarja az irhato felet
            syserr("close");
        }
        /* ez a folyamat olvas: amig a fiu nem ir a csobe,
           addig a folyamat a read-nel blokkolva van 
           a read akkor ter vissza, ha MAX karaktert olvasott
           vagy a masik fel lezarja a csovezeteket
           */
        int n;
        if ((n = read(pfd[0], b, MAX)) < 0) {
            syserr("read");
        }

        printf("az apa kiolvasta amit fiú folyamat írt: %s\n", b);

        if (close(pfd[0])) {
            syserr("close2");
        }

        wait(NULL); /* fiura var */
        exit(0);
    }
}
