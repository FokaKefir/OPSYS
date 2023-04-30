/* állapítsuk meg, mit csinál a program?
 */

#include <stdio.h>
#include <stdlib.h> 

#include "dump.h"

#define MAXBUFF 80

int main (int argc, char * argv [] )
{
    char buff[MAXBUFF] ;

    while (1) {

        printf("Írjon be egy sztringet:");

        if ( fgets(buff, MAXBUFF, stdin) == NULL ) {
            printf("nincs bemeneti sztring vagy hiba\n");
            break;
        }else{
            dump(buff,MAXBUFF);
        }
    
    }

    exit(EXIT_SUCCESS);
}    
