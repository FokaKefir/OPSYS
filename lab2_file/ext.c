/* 
 * Operacios rendszer II Labor
 * peldaprogram az lseek hasznalatara
 * a program az allomany vegen tul pozicionalja az offset
 * valtozot es utana ir 
 *
 */

#include <errno.h>	  
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>	  

#define LEN 80

char buff[LEN];

int main (int argc, char * argv[])
{
    int n;
    int i;
    int fd;

    char * nev = "teszt.dat" ; /* a fajl neve */
    buff[0] = 1; /* ezt a kodot irjuk be az elso irassal */

    /* megnyitjuk iras/olvasasra, letrehozzuk mint uj fajlt 
     * ha nem létezik, 
     * toroljuk a tartalmat ha letezik
     */

    if ( (fd=open( nev,  O_CREAT | O_RDWR | O_TRUNC , 0644)) < 0){
        perror("open");
        exit(1);
    }

    printf ("A fájl neve %s\n", nev);

    /* 1 karakter fajlba irasa */
    if ( (n=write ( fd, buff,1)) < 0) {
        perror("write");
        exit(1);
    }
    /* eloreviszem az offsetet a 101.-dik poziciora */
    if ( (i=lseek ( fd, 100, SEEK_CUR)) <0) {
        perror("lseek");
        exit (1);
    }
    printf("a seek %d -t adott vissza\n", i);

    /* rairok meg egy karaktert */
    *buff = 16;
    if ( (n=write ( fd, buff,1)) < 0) {
        perror("write");
        exit(-1);
    }
    /* a vegso allomany hossza 102 karakter lesz */

    printf("a write %d -t adott vissza\n",n);

    /* fajl lezarva */
    if (close(fd)<0){
        perror("close");
        exit(1);
    }

    /* ismet megnyitom */
    if ( (fd=open ( nev,  O_RDWR )) < 0){
        perror("open2");
        exit(1);
    }

    /* lekerem az offsetet az allomany vegerol */
    if ( (i = lseek (fd, 0, SEEK_END)) < 0 ) {
        perror("lseek");
        exit(1);
    }

    printf ("az allomany hossza %d byte\n",i);

    if (close(fd)<0){
        perror("close");
        exit(1);
    }

    exit(0);    
} 
