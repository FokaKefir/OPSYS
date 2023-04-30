#include <stdio.h>

/* 
 * állapítsuk meg, mit csinál a függvény?
 */

#define NEWROW 4

void dump ( char * buff, int len )
{
    int i;        
    char * s = buff;
    for (i=0; i<len; i++,s++) {
        if (!( i % NEWROW )) {
           printf ( "\n%2d ", i);     
        }        

        printf ("%02x ", *s );

        if ( *s == '\0' )
            break;
    }
    printf("\n");

    return ;
}   

