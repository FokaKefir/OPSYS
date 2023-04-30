/* 
 * egyszeru cat program
 *
 * hasznalat:
 *    mycat [ fajl ] ...
 * ha nem adunk meg fajlnevet, akkor a standard bemenetet masolja
 *
 * a ket eset van: 1. van fajlnev  2. nincs fájlnév 
 * ez egy if ket agaban van megoldva
 * feladat:
 * irjuk at a programot ugy, hogy a masolast a ket esetben ugyanaz a fuggveny vegezze!
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>	  
#include <errno.h>	  

#define LEN 80  //olvasasi puffer hossz

int main (int argc, char * argv[])
{
    char buff[LEN];//puffer az olvasashoz
    int n;
    int i;
    int fd; //fajl azonosito

    if (argc==1) {      // ha nincs parameter a stdin-t masoljuk 

        //a read a terminalrol valo olvasasnal akkor jon vissza
        //ha Enter-t kap
        //ha a terminálon tobb karaktert utunk be mint LEN , akkor
        //egymas utan tobb read fut le, 
        //es a tobb karakter a terminal driverben pufferelodik 
        //egy read maximum LEN karaktert ad vissza
        while ( (n = read (STDIN_FILENO, buff, LEN))>0 ){
            if (write ( STDOUT_FILENO , buff, n)<0){
                perror("write stdout");
                exit(1);
            }
        }
        if (n<0) { //ha n<0 hiba történt
            perror("olvasasi hiba");
            exit(1);
        }
    }else
    {   /* ha vannak parameterek a fajlokat masoljuk */
        /* az argc pont a maximalis indexet jelenti a parameter listaban */
        /* i=1 indexen van az elso fajlnev, az argv[i] helyen  */ 	
        /* mert az argv[0] a program nevet tartalmazza */
        for (i=1; i<argc; i++) { /* amig van meg parameter */
            /* nyitas csak olvasasra */
            if ( ( fd = open (argv[i], O_RDONLY )) < 0) { 
                perror("open");
                fprintf (stderr,"nem lehet megnyitni a %s allomanyt\n",argv[i]);
                exit(1);
            }
            //fprintf(stdout,"masol:%s\n",argv[i]);
            /* masolas */
            while ( (n = read (fd, buff, LEN))>0 ) {
                if (write ( STDOUT_FILENO , buff, n)<0){ //n karakter irasa az stdout-ra
                    perror("write");
                    exit(1);
                }
            }
            if (n<0) {
                perror("olvasasi hiba");
                exit(1);
            }
            if (close(fd)<0){ //zarjuk 
                perror("close");
                exit(1);
            }
        }
    }					
    exit(0);					
}
