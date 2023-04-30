/* pelda az execvp hasznalatara */

#include "myinclude.h"

//a parancssort feldolgozo fuggveny
//bemenet: parancssor sztring, pl:
//   ls -l /home
//kimenet: hibakod, ha nem hajtodik végre a parancs   
int parancssor ( const char * parancs)
{
    const char * elvalaszto = " ";
    int argc=0;
    char ** argv = NULL ; /*ebben lesz az argv*/
    char * masolat = NULL;
    char * s;

    /* mivel a strtok tonkreteszi a sztringet amin dolgozik, 
     * egy masolatot kell kesziteni rola */
    masolat = (char *) malloc ( strlen(parancs) +1);
    if (masolat==NULL)
        return -1;
    strcpy(masolat, parancs);
    argc=0;
    s=strtok (masolat, elvalaszto);
    while ( s != NULL ){
        /* van meg egy parameter, megnovelem az argv[]-t */
        argv = (char **) realloc (argv, (size_t) (argc+1)*(sizeof(*argv)));
        /* helyet foglalok a kovetkezo stringnek */
        argv[argc] = (char*) malloc ((size_t) strlen(s)+1);

        /*if (argv[argc] == NULL)
          return -1;
          */
        strcpy (argv[argc], s);
        argc++;
        s=strtok (NULL, elvalaszto); 
        /* a masodik hivasnal NULL az elso parameter */
    }
    /* amikor befejezzuk, az utolso mutatonal vagyunk, meg be kell irni egyet */
    argv = (char **) realloc (argv, (size_t) (argc+1)*(sizeof(*argv)));
    argv[argc]=NULL;
    /* itt megvan az argv */
    execvp (argv[0], argv);
    /* ide akkor jutunk ha nem sikerul */
    /* felszabditjuk a memoriat */
    while (--argc>0)
        free (argv[argc]);
    free (masolat);
    return 1;
} 

#define MAX 256  //egy parancssor hossza

int main (int argc, char * argv[])
{
    pid_t pid;
    int status;
    char parancs[MAX];

    setbuf(stdout,NULL);

    /* parancssor beolvasasa: */
    printf("írjunk be egy parancsot(például: ls -l /home ):");
    if ( fgets (parancs, MAX, stdin) == NULL){    /*olvasas stdin-rol*/
        printf("olvasás hiba\n");
        exit(1);
    }

    //az fgets() altal beolvasott sztring vegen
    //van egy ujsor, azt felulirjuk
    parancs[strlen(parancs)-1]='\0';

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
    exit(EXIT_SUCCESS);

}
