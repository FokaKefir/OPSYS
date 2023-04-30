#include <stdio.h>
#include <stdlib.h>
/*
 * példaprogram a parancssor argumentumainak 
 * egyszerű kezelésére
 */

//az environ egy külső változó
//a környezeti változókat tartalmazó 
//sztring listára mutat
//
extern char ** environ;

//az argc a paraméterek számát,
//az argv a paraméter listát tartalmazza
// argv -> argv[0] -> paraméter 0, a program neve
//         argv[1] -> paraméter 1
//         argv[2] -> paraméter 2
//   ...
//         NULL      utolsó elem 
//
int main( int argc, char * argv[] )
{
	int i;
    // a parancssor paramétereinek végigjárása 
    // a paraméterek sztringek !!!    
	for (i=0; i< argc;i++)
		printf ("%s\n",argv[i]);

    //környezeti változók végigjárása
    //hogy néz ki az environ lista?
 	//for ( ; *environ != NULL; environ++){
	//	printf ("%s\n",*environ);
	//}	
    
    exit(EXIT_SUCCESS);
}	
