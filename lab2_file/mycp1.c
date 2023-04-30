/* Operacios rendszer II Labor
 * egyszeru allomanymasolo program
 * inditas: mycp file1 file2
 * ez a valtozat nem masol ra egy mar letezo allomanyra
 */

#include <stdio.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

//hasznaljuk az errno valtozot, amelyben a rendszerfuggvenyek
//a hibakodot hagyjak
#include <errno.h>

#define BUFFSIZE 80 //a hasznalt puffer hossza 80 byte

extern int errno; //az errno kulso valtozo

int main ( int argc, char * argv[] )
{
    int fd1, fd2, n;
    char buf[BUFFSIZE];


    //harom parameter kell legyen
    if (argc != 3) {
        printf ("használat: %s f1 f2\n",argv[0]);
        exit(1);
    }

    //ha  a ket nev megegyezik, hiba
    if (! strcmp(argv[1],argv[2]) ){
        printf("%s: a két fájlnév megegyezik!\n", argv[0]);
        exit(1);
    }


    //az elso allomany nyitasa olvasasra
    if ( (fd1 = open (argv[1], O_RDONLY)) == -1 ) {
        perror("open1");
        exit(1);
    }

    //masodik allomany nyitasa irasra, de O_EXCL opcioval
    //ha letezik a fajl, akkor hibat ad
    if ( (fd2 = open ( argv[2], O_CREAT | O_EXCL | O_WRONLY , 0644)) == -1 )  {
        perror("open2");
        //az errno valtozoban jelenik meg a hibakod
        //a letezo hibakodokat lasd man 2 open
        if ( errno == EEXIST )
            printf("a cél fájl már  létezik\n");
        exit(1);
    }

    //masolas maximum BUFFSIZE szakaszokban
    while ( (n = read ( fd1, buf, BUFFSIZE ) ) > 0) {
        if ( (write ( fd2, buf, n) != n) ) {
            perror("write");
            exit(1);
        }
    }

    if (n<0){
        perror("read");
        exit(1);
    }
    //a vegen mindent lezarni !!!

    if (close(fd1) == -1 ){
            perror("write");
            exit(1);
    }
    if (close(fd2) == -1 ){
            perror("write");
            exit(1);
    }

    exit(0);
}
