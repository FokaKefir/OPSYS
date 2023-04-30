/* 
 * pelda az execvp hasznalatara 
 * egyszerubb az argv eloallitas, mint az execvp.c-ben
 * viszont korlatozzuk a hosszat
 * */

#include "myinclude.h"

#define MAX 256  //egy parancssor hossza

int main (int argc, char * argv[])
{
    pid_t pid;
    int status;
    char parancs[MAX];

    setbuf(stdout,NULL);

    /* parancssor beolvasasa: */
    while (1) {
        printf("ok> ");
        if ( fgets (parancs, MAX, stdin) == NULL){    /*olvasas stdin-rol*/
            fprintf(stderr,"olvasás hiba\n");
            exit(EXIT_FAILURE);
        }
        

        //az fgets altal beolvasott sztring vegen
        //van egy ujsor, azt felulirjuk
        parancs[strlen(parancs)-1]='\0';

        if (strcmp(parancs, "exit") == 0) {
            break;
        }

        pid = fork();
        if ( pid < 0){
            syserr("fork");
        }

        if ( pid == 0){

            /* fiu folyamat, uj programot inditunk */
            parancssor(parancs);

            /* ide mar nem ter vissza */
            syserr ("execv hiba\n");
        } 


        /*apa folyamat megvarja a fiut */
        wait (&status);
    }
    exit(EXIT_SUCCESS);

}
