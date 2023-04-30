#include "myinclude.h"
#include <limits.h>
#include "randlimit.h"
#include <time.h>

//generál N random egész számot és kiírja binárisan őket 
//egy fájlba egymás után
//paraméterek:  
//    ./create_rand fájlnév N M
//    N - legnagyobb szám
//    M - hány számot generáljon
//
//    fordítás:
//    gcc create_rand.c randlimit.c -o create_rand
/*
 * példa futtatás:
 *
 $ gcc create_rand.c randlimit.c -o create_rand -Wall
 $ ./create_rand x.dat 100 10
 $ hexdump  x.dat
 0000000 0054 0000 0027 0000 004e 0000 004f 0000
 0000010 005b 0000 0013 0000 0021 0000 004c 0000
 0000020 001b 0000 0037 0000                    
 0000028
 $ hexdump -d x.dat
 0000000   00084   00000   00039   00000   00078   00000   00079   00000
 0000010   00091   00000   00019   00000   00033   00000   00076   00000
 0000020   00027   00000   00055   00000                                
 0000028
 * egy int belső ábrázolásban ezen a gépen amelyen a példa futott
 *  * 32 bit = 8 * 4 bit 
 *
 */

int main (int argc, char * argv[])
{

    int N, M, R;
    int fd; //fájl

    if (argc!=4){
        fprintf(stderr,"használat: %s fájlnév legnagyobb_szám darab\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    N=atoi(argv[2]);  M=atoi(argv[3]);

    printf("legnagyobb %d\n", 10000);

    //korlátok 1 és 10000
    if (N<1 || N>10000 || M < 1 || M > 10000){
        fprintf(stderr,"az egyik szám nem megfelelő, korlátok: 1-%d\n", 10000);
        exit(EXIT_FAILURE);
    }

    printf("generál %d számot, legnagyobb: %d kimenet %s\n",M,N,argv[1]);

    //csonkítja a fájlt ha létezik
    if ( (fd=open(argv[1],O_WRONLY | O_CREAT | O_TRUNC, 0644)) <0 ){
        syserr(argv[1]);
    }

    srand((int) time(NULL));//seed

    while ( M-- ){
        R = randlimit(N);
        //printf ("write: %d\n",R);
        if (write(fd,&R,sizeof(int))<0){
            syserr("write");
        }
    }

    if (close(fd)<0){
        syserr("close");
    }

    exit(EXIT_SUCCESS);

}
