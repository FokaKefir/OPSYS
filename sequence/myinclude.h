
#define MYINCLUDE

#define _XOPEN_SOURCE  501
#include <sys/stat.h>  
#include <sys/unistd.h>  
#include <sys/types.h>
#include <fcntl.h>  
#include <stdlib.h>  
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>  
#include <errno.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <math.h>
#include <time.h>
 
#include <signal.h>

extern int errno ; /* az errno.h által visszadott hibakód */

//rendszer hiba kiíró függvény (makró)
//egy sztringet kap bemenetként, kiírja a sztringet és
//hozzáfűzi a rendszer hiba üzenetét:
//   msg : rendszerhiba
//   alakban

#define syserr(msg)   \
  do { \
       fprintf(stderr, "hiba: %s %d. sor:\n", __FILE__, __LINE__); \
       perror ( msg ); \
       exit (EXIT_FAILURE); \
     } while(0) 
   

