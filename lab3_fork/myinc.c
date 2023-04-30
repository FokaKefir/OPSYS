/*
 * a myinclude.h hasznalata
 */

//haszanaljuk a myinclude.h fejlecet
#include "myinclude.h"

int main() 
{
   int fd;
   setbuf(stdout,NULL);

   //ha nincs a.txt hibat fog adni
   if ( (fd = open ("aaa.txt",O_RDONLY)) < 0)  
         syserr("open hiba"); //ha ki is akarunk lepni, egyszerubb 
                             //a syserr()-t hivni a myinclude-bol

   printf("\n");
   close(fd);
   exit (0);
}
