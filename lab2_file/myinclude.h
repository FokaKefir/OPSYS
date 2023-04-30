#include <sys/unistd.h>  
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>  
#include <errno.h>
 
extern int errno ; /* az errno.h által visszaadott hibakód */

//rendszer hiba kiíró függvény
//egy sztringet kap bemenetként, kiírja a sztringet és
//hozzáfűzi a rendszer hiba üzenetét:
//   msg : rendszerhiba
//   alakban

/*
void syserr ( char * msg )  
{  
    fprintf(stderr, "hiba: %s %d. sor:\n", __FILE__, __LINE__); 
    perror ( msg ); // kiírja az errno változóban található hibakódot
                    //és egy rövid hiba üzenetet 
    exit (EXIT_FAILURE); 
}
*/

//makróval ugyanaz:
#define syserr(msg)   \
  do { \
       fprintf(stderr, "hiba: %s %d. sor:\n", __FILE__, __LINE__); \
       perror ( msg ); \
       exit (EXIT_FAILURE); \
     } while(0) 

